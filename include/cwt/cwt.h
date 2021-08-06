#ifndef CWT_CWT_H
#define CWT_CWT_H

#include "vector"

#include "wavelet/wavelet.hpp"

class CWT
{
public:
    CWT(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales);

    ~CWT();

    virtual bool execute_conv(float* in, float* out) = 0;
    virtual bool execute_fft(float* in, float* out) = 0;

protected:
    const Wavelet& wavelet;
    const std::vector<float>& scales;

    int cols;
    int rows;
};

#endif //CWT_CWT_H
