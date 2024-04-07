#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <math.h>
#include <string>

struct Matrix
{
    int nrows, ncols;
    float *data;

    Matrix(int _nrows, int _ncols) : nrows(_nrows), ncols(_ncols)
    {
        data = new float[nrows * ncols];
    }

    Matrix(const std::initializer_list<std::initializer_list<float>> &values)
    {
        nrows = values.size();
        ncols = values.begin()->size();
        data = new float[nrows * ncols];
        int index = 0;
        for (auto row : values)
        {
            for (auto value : row)
            {
                data[index++] = value;
            }
        }
    }

    // Matrix &operator=(const std::initializer_list<std::initializer_list<float>> &values)
    // {
    //     nrows = values.size();
    //     ncols = values.begin()->size();
    //     free_data();
    //     data = new float[nrows * ncols];
    //     int index = 0;
    //     for (auto row : values)
    //     {
    //         for (auto value : row)
    //         {
    //             data[index++] = value;
    //         }
    //     }
    //     return *this;
    // }

    static Matrix identity(int nrows)
    {
        Matrix res = Matrix(nrows, nrows);
        for (int i = 0; i < nrows; i++)
        {
            for (int j = 0; j < nrows; j++)
            {
                if (i == j)
                {
                    res.data[i * nrows + j] = 1;
                }
                else
                {
                    res.data[i * nrows + j] = 0;
                }
            }
        }
        return res;
    }

    float *operator[](int row)
    {
        return &data[row * ncols];
    }

    const float *operator[](int row) const
    {
        return &data[row * ncols];
    }

    // ~Matrix()
    // {
    //     delete[] data;
    // }

    Matrix operator*(const Matrix &m)
    {
        Matrix res = Matrix(nrows, m.ncols);
        for (int i = 0; i < nrows; i++)
        {
            for (int j = 0; j < m.ncols; j++)
            {
                res[i][j] = 0;
                for (int k = 0; k < ncols; k++)
                {
                    res[i][j] += data[i * ncols + k] * m[k][j];
                }
            }
        }
        return res;
    }

    std::string to_string()
    {
        std::string res = "";
        for (int i = 0; i < nrows; i++)
        {
            for (int j = 0; j < ncols; j++)
            {
                res += std::to_string(data[i * ncols + j]) + " ";
            }
            res += "\n";
        }
        return res;
    }
};

#endif // _MATRIX_HPP