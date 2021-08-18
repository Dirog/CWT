/// \file wavelet.hpp
/// \author Denis Kozlov
#ifndef CWT_WAVELET_HPP
#define CWT_WAVELET_HPP

#include <vector>

/// \brief Класс для работы с вейвлетами.
/// Класс создает матрицу вейвлетов по заданным параметрам.
/// По строкам матрицы уложены отчеты масштабированных вейвлетов.
class Wavelet
{
    /// Задаваемая пользователем функция, описывающая вейвлет.
    /// Принимает временной индекс и коэффициент масштаба.
    /// Возвращает значение вейвлета для заданных параметров.
    using Func = float (*)(float, float);

public:
    /// \brief Конструктор.
    /// \param[in] func -- функция вейвлета.
    /// \param[in] fs_hz -- частота дискретизации в Гц.
    /// \param[in] len -- длинна вейвлета в отчетах.
    /// \param[in] scales -- вектор коэффицентов масштаба.
    Wavelet(Func func, float fs_hz, int len, std::vector<float> scales);
    /// \brief Конструктор.
    Wavelet(Func func, float fs_hz, int signallen, int maxscale);
    /// \brief Деструктор.
    ~Wavelet();
    /// \brief Функция, возвращающающая заданную размерность матрицы вейвлетов.
    /// \param[in] dim -- размерность (0 или 1).
    /// \note При dim == 0 возвращается количество столбцов в матрице.
    /// \note При dim == 1 возвращается количество строк в матрице.  
    int shape(int dim) const;
    /// \brief Вернуть указатель на данные вейвлетной матрицы.
    float* ptr() const;
    /// \brief Вернуть вектор коэффициентов масштаба.    
    std::vector<float> scales() const;

private:
    /// Вектор коэффициентов масштаба.
    std::vector<float> scales_;
    /// Указатель на данные вейвлетной матрицы.
    float* data;
    /// количество столбцов в матрице.
    int cols;
    /// количество строк в матрице.
    int rows;
};

#endif //CWT_WAVELET_HPP
