#include <cmath>
#include "memory"

#include "wavelet/wavelet.hpp"

Wavelet::Wavelet(Func func, float fs, int len, std::vector<float> scales)
    : cols(len), rows(scales.size())
{
    data = new float[cols * rows];
    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            data[i + j * cols] = func( (i - (cols - 1.0) / 2.0) / fs, scales[j] );
        }
    }
}

Wavelet::Wavelet(Wavelet::Func func, float fs, int signallen, int maxscale)
{
    rows = maxscale;
    cols = (int)fmin(10 * rows, signallen);

    std::vector<float> scales;
    for (int i = 0; i < rows; ++i)
    {
        scales.push_back(i + 1);
    }

    scales_ = scales;

    data = new float[cols * rows];
    for (int i = 0; i < cols; ++i)
    {
        for (int j = 0; j < rows; ++j)
        {
            data[i + j * cols] = func( (i - (cols - 1.0) / 2.0) / fs, scales[j] );
        }
    }
}

Wavelet::~Wavelet()
{
    free(data);
}

int Wavelet::shape(int dim) const
{
    if(dim == 0)
        return cols;
    else if(dim == 1)
        return rows;

    return -1;
}

float* Wavelet::ptr() const
{
    return data;
}

std::vector<float> Wavelet::scales() const
{
    return scales_;
}

