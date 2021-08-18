/// \file cwt.cpp
/// \author Denis Kozlov
#include <chrono>
#include <cufft.h>
#include <stdio.h>
#include <iostream>
#include <cwt/cuda/cwt.h>


#define BLOCK_DIM 128
namespace Kernels
{
    __global__ void conv(float *out, const float *in, const float *filter, int cols, int rows, int fcols, int frows)
    {
        int idx_x = blockDim.x * blockIdx.x + threadIdx.x;
        int idx_y = blockDim.y * blockIdx.y + threadIdx.y;

        int pad_rows = frows / 2;
        int pad_cols = fcols / 2;
        int tile_rows = BLOCK_DIM + 2 * pad_rows;
        int tile_cols = BLOCK_DIM + 2 * pad_cols;

        extern __shared__ float s_input[];

        for (int row = 0; row <= tile_rows / BLOCK_DIM; row++)
        {
            for (int col = 0; col <= tile_cols / BLOCK_DIM; col++)
            {
                int idx_row = idx_y + BLOCK_DIM * row - pad_rows;
                int idx_col = idx_x + BLOCK_DIM * col - pad_cols;
                int fid_row = threadIdx.y + BLOCK_DIM * row;
                int fid_col = threadIdx.x + BLOCK_DIM * col;

                if (fid_row >= tile_rows || fid_col >= tile_cols)
                {
                    continue;
                }

                s_input[tile_cols * fid_row + fid_col] = \
                        (idx_row >= 0 && idx_row < rows && idx_col >= 0 && idx_col < cols) ?
                        in[cols * idx_row + idx_col] : 0.f;
            }
        }

        __syncthreads();

        if (idx_x < cols)
        {
            if (idx_y < rows)
            {
                float result = 0.f;
                for (int filter_row = -frows / 2; filter_row <= frows / 2; ++filter_row)
                {
                    for (int filter_col = -fcols / 2; filter_col <= fcols / 2; ++filter_col)
                    {
                        int image_row = threadIdx.y + pad_rows + filter_row;
                        int image_col = threadIdx.x + pad_cols + filter_col;

                        float image_value = s_input[tile_cols * image_row + image_col];
                        float filter_value = filter[(filter_row + frows / 2) * fcols + filter_col + fcols / 2];

                        result += image_value * filter_value;
                    }
                }

                int index = idx_y * cols + idx_x;
                out[index] = result;
            }
        }
    }

    __global__
    void rearrange(float* input, float* output, int cols, int rows, int frows)
    {
        int i = blockDim.x * blockIdx.x + threadIdx.x;

        int col  = i % cols;
        int row  = (i / cols) % rows;
        int frow = i / (cols * rows);

        if (frow < frows)
        {
            int in_idx  = col + cols * row + cols * rows * frow;
            int out_idx = col + cols * frow + cols * frows * row;
            output[out_idx] = input[in_idx];

        }
    }

    __inline__ __device__
    cufftComplex operator*(cufftComplex const& a, cufftComplex const& b) {
        cufftComplex c;
        c.x = a.x * b.x - a.y * b.y;
        c.y = a.x * b.y + a.y * b.x;
        return c;
    }

    __global__ void multiply(cufftComplex* in, cufftComplex* wavelet, cufftComplex* out, int len)
    {
        int i = blockDim.x * blockIdx.x + threadIdx.x;

        if (i < len)
        {
            out[i] = in[i] * wavelet[i];
        }
    }

    void createStreams(cudaStream_t *stream, int count)
    {
        for (int i = 0; i < count; i++)
        {
            cudaStreamCreate(&stream[i]);
        }
    }
}

CUDA::CUDA(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales)
    : CWT(cols, rows, wavelet, scales)
{

}

CUDA::~CUDA()
{

}

bool execute_conv(const float* in, float* out, int cols, int rows, const Wavelet& wavelet)
{
    auto err = cudaSetDevice(0);
    if (err != 0) std::cout << err << std::endl;
    int fcols = wavelet.shape(0);
    int frows = wavelet.shape(1);

    float *d_in;
    float *d_out;
    float *d_out_rearranged;
    float *d_wavelet;


    err = cudaMalloc(&d_in, cols * rows * sizeof(float));
        if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&d_out, cols * rows * frows * sizeof(float));
        if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&d_out_rearranged, cols * rows * frows * sizeof(float));
    if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&d_wavelet, fcols * frows * sizeof(float));
        if (err != 0) std::cout << err << std::endl;

    err = cudaMemcpy(d_in, in, cols * rows * sizeof(float), cudaMemcpyHostToDevice);
        if (err != 0) std::cout << err << std::endl;

    err = cudaMemcpy(d_wavelet, wavelet.ptr(), fcols * frows * sizeof(float), cudaMemcpyHostToDevice);
        if (err != 0) std::cout << err << std::endl;


    cudaStream_t stream[frows];
    Kernels::createStreams(stream, frows);

    auto start = std::chrono::high_resolution_clock::now();

    dim3 block(BLOCK_DIM, 1);
    dim3 grid(std::ceil((float) (cols + BLOCK_DIM - 1) / BLOCK_DIM), rows);

    int shared_mem = (2 * fcols + BLOCK_DIM) * sizeof(float);

    for (int i = 0; i < frows; i++)
    {
        // Shape frows x rows x cols !
        Kernels::conv<<<grid, block, shared_mem>>>
            (d_out + i * cols * rows, d_in, d_wavelet + i * wavelet.shape(0), cols, rows, wavelet.shape(0), 1);
    }


    int blocks = ceil((float) cols * rows * frows / 128.0f);
    Kernels::rearrange<<<blocks, 128>>>(d_out, d_out_rearranged, cols, rows, frows);

    cudaDeviceSynchronize();
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << "CUDA Conv Elapsed time: " << duration_us.count() << "us\t" << duration_ms.count() << "ms\n";

    err = cudaMemcpy(out, d_out_rearranged, cols * rows * frows * sizeof(float), cudaMemcpyDeviceToHost);
            if (err != 0) std::cout << err << std::endl;

    cudaFree(d_in);
    cudaFree(d_out);
    cudaFree(d_wavelet);
    return true;
}
//
bool execute_fft(const float* in, float* out, int cols, int rows, const Wavelet& wavelet)
{
    auto err = cudaSetDevice(0);
    if (err != 0) std::cout << err << std::endl;
    int fcols = wavelet.shape(0);
    int frows = wavelet.shape(1);

    float *d_in;
    cufftComplex *fft_in;

    float *d_out;
    cufftComplex *fft_out;

    float *d_wavelet;
    cufftComplex *fft_wavelet;

    int M = (floor(cols / 2) + 1);
    err = cudaMalloc(&d_in, cols * rows * sizeof(float));
    if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&fft_in, M * rows * sizeof(cufftComplex));
    if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&d_out, cols * rows * frows * sizeof(float));
    if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&fft_out, M * rows * frows * sizeof(cufftComplex));
    if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&d_wavelet, cols * frows * sizeof(float));
    if (err != 0) std::cout << err << std::endl;

    err = cudaMalloc(&fft_wavelet, M * frows * sizeof(cufftComplex));
    if (err != 0) std::cout << err << std::endl;


    err = cudaMemcpy(d_in, in, cols * rows * sizeof(float), cudaMemcpyHostToDevice);
    if (err != 0) std::cout << err << std::endl;

    cudaMemset(d_wavelet, 0.0f, cols * frows);
    for (int i = 0; i < frows; i++)
    {
        err = cudaMemcpy(d_wavelet + i * cols, wavelet.ptr() + i * fcols, fcols * sizeof(float), cudaMemcpyHostToDevice);
        if (err != 0) std::cout << err << std::endl;
    }


//    cudaStream_t stream[scales.size()];
//    Kernels::createStreams(stream, scales.size());

    cufftHandle signal_plan;
    auto cerr = cufftPlan1d(&signal_plan, cols, CUFFT_R2C, rows);
    if (cerr != 0) std::cout << cerr << std::endl;

    cufftHandle wavelet_plan;
    cerr = cufftPlan1d(&wavelet_plan, cols, CUFFT_R2C, frows);
    if (cerr != 0) std::cout << cerr << std::endl;

    cufftHandle out_plan;
    cerr = cufftPlan1d(&out_plan, cols, CUFFT_C2R, rows * frows);
    if (cerr != 0) std::cout << cerr << std::endl;


    auto start = std::chrono::high_resolution_clock::now();

    cerr = cufftExecR2C(signal_plan, d_in, fft_in);
    if (cerr != 0) std::cout << cerr << std::endl;

    cerr = cufftExecR2C(wavelet_plan, d_wavelet, fft_wavelet);
    if (cerr != 0) std::cout << cerr << std::endl;

    for (int c = 0; c < rows; c++)
    {
        for (int a = 0; a < frows; a++)
        {
            Kernels::multiply<<<M, 128>>>(fft_in + c * M, fft_wavelet + a * M, fft_out + a * M + c * M * frows, M);
        }
    }

    cerr = cufftExecC2R(out_plan, fft_out, d_out);
    if (cerr != 0) std::cout << cerr << std::endl;

    cudaDeviceSynchronize();
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << "CUDA FFT Elapsed time: " << duration_us.count() << "us\t" << duration_ms.count() << "ms\n";

    err = cudaMemcpy(out, d_out, cols * rows * frows * sizeof(float), cudaMemcpyDeviceToHost);
    if (err != 0) std::cout << err << std::endl;

    cufftDestroy(wavelet_plan);
    cufftDestroy(signal_plan);
    cufftDestroy(out_plan);
    cudaFree(d_in);
    cudaFree(d_out);
    cudaFree(d_wavelet);
    cudaFree(fft_in);
    cudaFree(fft_out);
    cudaFree(fft_wavelet);
    return true;
}


bool CUDA::execute(const float* in, float* out, Mode mode)
{
    bool status = false;
    if (mode == TimeDomain)
    {
        status = execute_conv(in, out, cols, rows, wavelet);
    }
    else if (mode == FrequencyDomain)
    {
        status = execute_fft(in, out, cols, rows, wavelet);
    }

    return status;
}