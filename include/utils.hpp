#ifndef CWT_UTILS_HPP
#define CWT_UTILS_HPP

#include <fstream>
#include <iostream>

namespace IO
{
    template<typename T>
    int FromFile(const std::string& filePath, T* output, std::size_t len)
    {
        using namespace std;
        ifstream rf(filePath, ios::binary);
        if (!rf)
        {
            cerr << "Cannot open file! " << filePath << endl;
            std::exit(-1);
        }

        for (int i = 0; i < len; i++)
        {
            rf.read((char*) &output[i], sizeof(T));
        }

        rf.close();
        if (!rf.good())
        {
            cerr << "Error occurred at reading time!" << endl;
            std::exit(-1);
        }
        return 0;
    }

    template<typename T>
    int ToFile(const std::string& filePath, T* input, std::size_t len)
    {
        using namespace std;
        ofstream wf(filePath, ios_base::out | ios_base::binary);
        if (!wf)
        {
            cerr << "Cannot open file!" << filePath << endl;
            std::exit(-1);
        }

        for (int i = 0; i < len; i++)
        {
            wf.write((char*) &input[i], sizeof(T));
        }

        wf.close();
        if (!wf.good())
        {
            cerr << "Error occurred at writing to file time!" << endl;
            std::exit(-1);
        }
        return 0;
    }
}

namespace Difference
{
    inline float Subtraction(const float* lhs, const float* rhs, int len)
    {
        float diff = 0.0f;
        for (int i = 0; i < len; ++i)
        {
            diff += lhs[i] - rhs[i];
        }

        return diff;
    }
}

#endif // CWT_UTILS_HPP
