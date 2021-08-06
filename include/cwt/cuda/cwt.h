#ifndef CUDA_CWT_H
#define CUDA_CWT_H

#include "cwt/cwt.h"

class CUDA : CWT
{
public:
    CUDA(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales);
    ~CUDA();

    bool execute_conv(float* in, float* out) final;
    bool execute_fft(float* in, float* out) final;
};

#endif //CUDA_CWT_H
