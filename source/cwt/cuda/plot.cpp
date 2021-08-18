/// \file plot.cpp
/// \author Denis Kozlov
#include <cmath>
#include <utils.hpp>
#include <cwt/cuda/cwt.h>
#include <CppUTest/TestHarness.h>

namespace
{
    float mex(float t, float s)
    {
        return 2 / ( sqrtf(3 * s) * powf(M_PI, 0.25) ) * ( 1 - powf(t / s, 2) ) * expf(-t*t / (2 * s*s));
    }
}


TEST_GROUP(PLOTCWT)
{

};


TEST(PLOTCWT, FFT)
{
   const int rows = 1;
   const int cols = 1024;
   const int frows = 64;

   const int inputlen = cols * rows;
   const int outputlen = inputlen * frows;

   float* input = new float[inputlen];
   float* output = new float[outputlen];

   auto path = "/home/dirog/Projects/CWT/source/test/plot/seicmic.bin";
   IO::FromFile(path, input, inputlen);

   Wavelet wavelet(mex, 1, cols, frows);

   CUDA cwt(cols, rows, wavelet, wavelet.scales());

   cwt.execute(input, output, CWT::TimeDomain);

   IO::ToFile("/home/dirog/Projects/CWT/source/test/plot/output.bin", output, outputlen);

   char buffer[100];
   std::sprintf(buffer, "python3 /home/dirog/Projects/CWT/scripts/python/plot.py %d %d %d %s",
                cols, rows, frows, path);
   system(buffer);
   free(input);
   free(output);
}


