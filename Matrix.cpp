#include "Matrix.hpp"
#include <iostream>
#include <cmath>

Matrix::Matrix(int _nrows, int _ncols) : nrows(_nrows), ncols(_ncols)
{
    data = new float[nrows * ncols];
}

Matrix::Matrix(const std::initializer_list<std::initializer_list<float>> &values)
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

Matrix Matrix::identity(int nrows)
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

float *Matrix::operator[](int row)
{
    return &data[row * ncols];
}

const float *Matrix::operator[](int row) const
{
    return &data[row * ncols];
}

Matrix Matrix::operator*(const Matrix &m)
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

std::string Matrix::to_string()
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

Matrix Matrix::copy_matrix()
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

bool Matrix::operator==(const Matrix &other)
{
    if (nrows != other.nrows || ncols != other.ncols)
    {
        return false;
    }
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            if (data[i * ncols + j] != other[i][j])
            {
                return false;
            }
        }
    }
    return true;
}

Matrix Matrix::submatrix(int row, int col)
{
    if (row >= nrows || col >= ncols)
    {
        std::cerr << "Index out of bounds\n";
        exit(1);
    }
    if (nrows == 1 || ncols == 1)
    {
        std::cerr << "Matrix is too small\n";
        exit(1);
    }
    Matrix res = Matrix(nrows - 1, ncols - 1);
    int index_i = 0;
    for (int i = 0; i < nrows; i++)
    {
        if (i == row)
            continue;
        int index_j = 0;

        for (int j = 0; j < ncols; j++)
        {
            if (j == col)
                continue;
            res[index_i][index_j] = (*this)[i][j];
            index_j++;
        }
        index_i++;
    }

    return res;
}

float Matrix::cofactor(int row, int col)
{
    Matrix cofactor_matrix = submatrix(row, col);
    return pow(-1, row + col) * cofactor_matrix.determinant();
}

float Matrix::determinant()
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
    float res = 0;
    for (int i = 0; i < ncols; i++)
    {
        res += data[i] * cofactor(0, i);
    }
    return res;
}

Matrix Matrix::inverse()
{
    if (nrows != ncols)
    {
        std::cerr << "Matrix is not square\n";
        exit(1);
    }
    float det = determinant();
    if (det == 0)
    {
        std::cerr << "Determinant is 0\n";
        exit(1);
    }

    if (nrows == 2)
    {
        Matrix res = Matrix(2, 2);
        res[0][0] = data[3] / det;
        res[0][1] = -data[1] / det;
        res[1][0] = -data[2] / det;
        res[1][1] = data[0] / det;
        return res;
    }

    Matrix res = Matrix(nrows, ncols);
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            res[i][j] = cofactor(j, i) / det;
        }
    }
    return res;
}

Matrix Matrix::transpose()
{
    Matrix res = Matrix(ncols, nrows);
    for (int i = 0; i < nrows; i++)
    {
        for (int j = 0; j < ncols; j++)
        {
            res[j][i] = data[i * ncols + j];
        }
    }
    return res;
}