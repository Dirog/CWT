#ifndef CWT_WAVELET_HPP
#define CWT_WAVELET_HPP

#include <vector>


class Wavelet
{
    using Func = float (*)(float, float);

public:
    Wavelet(Func func, float fs, int len, std::vector<float> scales);

    Wavelet(Func func, float fs, int signallen, int maxscale);

    ~Wavelet();

    int shape(int dim) const;

    float* ptr() const;

    std::vector<float> scales() const;

private:
    std::vector<float> scales_;
    float* data;
    int cols;
    int rows;

};

#endif //CWT_WAVELET_HPP
