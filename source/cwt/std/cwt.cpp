/// \file cwt.cpp
/// \author Denis Kozlov
#include <chrono>
#include <iostream>
#include <cwt/std/cwt.h>

void conv(float *out, const float *in, const float *filter, int cols, int fcols)
{
    for (int col = 0; col < cols; ++col)
    {
        float result = 0.f;
        for (int filter_col = -fcols / 2; filter_col <= fcols / 2; ++filter_col)
        {
            int image_col = col + filter_col;

            float image_value = (image_col >= 0 && image_col < cols) ?
                                in[image_col] : 0.f;

            float filter_value = filter[filter_col + fcols / 2];

            result += image_value * filter_value;
        }

        out[col] = result;
    }
}


STD::STD(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales)
    : CWT(cols, rows, wavelet, scales)
{

}

STD::~STD()
{

}

bool STD::execute(const float *in, float *out, Mode mode)
{
    if (mode == FrequencyDomain)
    {
        return false;
    }

    auto start = std::chrono::high_resolution_clock::now();
    int frows = wavelet.shape(1);
    for(int c = 0; c < rows; c++)
    {
        for (int i = 0; i < frows; i++)
        {
            conv(out + i * cols + c * cols * frows, in + c * cols,
                wavelet.ptr() + i * wavelet.shape(0),cols, wavelet.shape(0));
        }
    }
    auto finish = std::chrono::high_resolution_clock::now();
    auto duration_us = std::chrono::duration_cast<std::chrono::microseconds>(finish - start);
    auto duration_ms = std::chrono::duration_cast<std::chrono::milliseconds>(finish - start);
    std::cout << "STD Conv Elapsed time: " << duration_us.count() << "us\t" << duration_ms.count() << "ms\n";

    return true;
}

