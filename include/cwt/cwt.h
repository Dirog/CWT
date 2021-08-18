/// \file cwt.h
/// \author Denis Kozlov
#ifndef CWT_CWT_H
#define CWT_CWT_H

#include <vector>
#include <wavelet/wavelet.hpp>

/// \brief Класс непрерывного вейвлет преобразования (НВП).
class CWT
{
public:
    /// \brief Режим вычислений.
    enum Mode
    {
        /// Вычисления во временной области.
        TimeDomain = 0,
        /// Вычисления в частотной области с использованием БПФ.
        FrequencyDomain = 1
    };
    /// \brief Конструктор.
    /// \param[in] cols -- число физических каналов в сигнале.
    /// \param[in] rows -- число отчетов в сигнале.
    /// \param[in] wavelet -- экземпляр класса вейвлет.
    /// \param[in] scales -- вектор коэффицентов масштаба.
    CWT(int cols, int rows, const Wavelet& wavelet, const std::vector<float>& scales);
    /// \brief Деструктор.
    ~CWT();
    /// \brief Посчитать непрерывное вейвлет преобразование.
    /// \param[in] in -- указатель на входной многоканальный сигнал.
    /// \param[out] out -- указатель на выходной тензор (память выделяет пользователь).
    /// \param[in] mode -- режим вычисления: во временной или частотной области.
    /// \return -- флаг, показывающий успешно ли прошли вычисления.
    virtual bool execute(const float* in, float* out, Mode mode) = 0;

protected:
    /// Экземпляр класса вейвлет.
    const Wavelet& wavelet;
    /// Вектор коэффицентов масштаба.
    const std::vector<float>& scales;
    /// Количество физических каналов в сигнале.
    int cols;
    /// Число отчетов в сигнале.
    int rows;
};

#endif //CWT_CWT_H
