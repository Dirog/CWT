#include "cwt/cwt.h"

CWT::CWT(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales)
    : wavelet(wavelet), scales(scales), cols(cols), rows(rows)
{

}

CWT::~CWT()
{

}

