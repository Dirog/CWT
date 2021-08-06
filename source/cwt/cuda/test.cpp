#include <cmath>

#include "cwt/cuda/cwt.h"
#include "utils.hpp"

#include <CppUTest/TestHarness.h>

namespace
{
    float mex(float t, float s)
    {
        return 2 / ( sqrtf(3 * s) * powf(M_PI, 0.25) ) * ( 1 - powf(t / s, 2) ) * expf(-t*t / (2 * s*s));
    }
}



TEST_GROUP(CWTCUDA)
{

};

TEST(CWTCUDA, Conv)
{
    const int rows = 2;
    const int cols = 16384;
    const int frows = 64;

    const int inputlen = cols * rows;
    const int outputlen = inputlen * frows;

    float* input = new float[inputlen];
    float* output = new float[outputlen];
    float* expected = new float[outputlen];

    IO::FromFile("/home/dirog/Projects/CWT/source/test/data/input.bin", input, inputlen);
    IO::FromFile("/home/dirog/Projects/CWT/source/test/data/output_conv.bin", expected, outputlen);

    Wavelet wavelet(mex, 1, cols, frows);

    CUDA cwt(cols, rows, wavelet, wavelet.scales());

    cwt.execute_conv(input, output);

    float diff = Difference::Subtraction(output, expected, outputlen);

    DOUBLES_EQUAL(0.0f, diff, 1e-3);

    free(input);
    free(output);
    free(expected);
}

TEST(CWTCUDA, FFT)
{
    const int rows = 2;
    const int cols = 16384;
    const int frows = 64;

    const int inputlen = cols * rows;
    const int outputlen = inputlen * frows;

    float* input = new float[inputlen];
    float* output = new float[outputlen];
    float* expected = new float[outputlen];

    IO::FromFile("/home/dirog/Projects/CWT/source/test/data/input.bin", input, inputlen);
    IO::FromFile("/home/dirog/Projects/CWT/source/test/data/output_fft.bin", expected, outputlen);

    Wavelet wavelet(mex, 1, cols, frows);

    CUDA cwt(cols, rows, wavelet, wavelet.scales());

    cwt.execute_fft(input, output);

    float diff = Difference::Subtraction(output, expected, outputlen);

    DOUBLES_EQUAL(0.0f, diff, 1e-3);

    free(input);
    free(output);
    free(expected);
}