/// \file cwt.h
/// \author Denis Kozlov
#ifndef CUDA_CWT_H
#define CUDA_CWT_H

#include <cwt/cwt.h>

/// \brief Класс НВП с вычислением на графическом процессоре.
class CUDA : CWT
{
public:
    /// \brief Конструктор.
    /// \param[in] cols -- число физических каналов в сигнале.
    /// \param[in] rows -- число отчетов в сигнале.
    /// \param[in] wavelet -- экземпляр класса вейвлет.
    /// \param[in] scales -- вектор коэффицентов масштаба.
    CUDA(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales);
    /// \brief Деструктор.
    ~CUDA();
    /// \brief Посчитать непрерывное вейвлет преобразование на графическом процессоре.
    /// \param[in] in -- указатель на входной многоканальный сигнал.
    /// \param[out] out -- указатель на выходной тензор (память выделяет пользователь).
    /// \param[in] mode -- режим вычисления: во временной или частотной области..
    /// \return -- флаг, показывающий успешно ли прошли вычисления.
    bool execute(const float* in, float* out, Mode mode) final;
};

#endif //CUDA_CWT_H
