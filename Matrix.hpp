#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <math.h>
#include <string>
#include <iostream>

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

    Matrix copy_matrix()
    {
        Matrix res = Matrix(nrows, ncols);
        for (int i = 0; i < nrows; i++)
        {
            for (int j = 0; j < ncols; j++)
            {
                res[i][j] = data[i * ncols + j];
            }
        }
        return res;
    }

    float determinant()
    {
        if (nrows != ncols)
        {
            std::cerr << "Matrix is not square\n";
            exit(1);
        }
        if (nrows == 2)
        {
            return (*this)[0][0] * (*this)[1][1] - (*this)[0][1] * (*this)[1][0];
        }

        float res = 1;
        Matrix copy = copy_matrix();
        for (int i = 0; i < nrows; i++)
        {
            // Pivot research
            int pivot = i;
            for (int j = i + 1; j < nrows; j++)
            {
                if (std::abs(copy[j][i]) > std::abs(copy[pivot][i]))
                {
                    pivot = j;
                }
            }

            // Pivot not on the diagonal so we need to swap rows
            if (pivot != i)
            {
                Matrix tmp = Matrix(nrows, ncols);

                for (int j = 0; j < ncols; j++)
                {
                    tmp[i][j] = copy[i][j];
                    copy[i][j] = copy[pivot][j];
                    copy[pivot][j] = tmp[i][j];
                }

                // Changed rows so we need to change the sign of the determinant
                res *= -1;
            }

            // Matrix is singular
            if (copy[i][i] == 0)
            {
                return 0;
            }

            res *= copy[i][i];

            // Gauss elimination
            for (int j = i + 1; j < nrows; j++)
            {
                double factor = copy[j][i] / copy[i][i];
                for (int k = i + 1; k < nrows; k++)
                {
                    copy[j][k] -= factor * copy[i][k];
                }
            }
        }

        return res;
    }
};

#endif // _MATRIX_HPP