#ifndef STD_CWT_H
#define STD_CWT_H

#include "cwt/cwt.h"

class STD : CWT
{
public:
    STD(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales);
    ~STD();

    bool execute_conv(float* in, float* out) final;
    bool execute_fft(float* in, float* out) final;
};

#endif //STD_CWT_H
