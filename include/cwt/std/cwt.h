/// \file cwt.h
/// \author Denis Kozlov
#ifndef STD_CWT_H
#define STD_CWT_H

#include <cwt/cwt.h>


/// \brief Класс НВП с вычислением на процессоре.
class STD : CWT
{
public:
    /// \brief Конструктор.
    /// \param[in] cols -- число физических каналов в сигнале.
    /// \param[in] rows -- число отчетов в сигнале.
    /// \param[in] wavelet -- экземпляр класса вейвлет.
    /// \param[in] scales -- вектор коэффицентов масштаба.
    STD(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales);
    /// \brief Деструктор.
    ~STD();
    /// \brief Посчитать непрерывное вейвлет преобразование на процессоре.
    /// \param[in] in -- указатель на входной многоканальный сигнал.
    /// \param[out] out -- указатель на выходной тензор (память выделяет пользователь).
    /// \param[in] mode -- необходимо выставить TimeDomain. FrequencyDomain не поддерживается.
    /// \return -- флаг, показывающий успешно ли прошли вычисления.
    bool execute(const float* in, float* out, Mode mode = TimeDomain) final;
};

#endif //STD_CWT_H
