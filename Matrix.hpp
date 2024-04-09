#ifndef _MATRIX_HPP
#define _MATRIX_HPP

#include <initializer_list>
#include <string>

struct Matrix
{
    int nrows, ncols;
    float *data;

    Matrix(int _nrows, int _ncols);
    Matrix(const std::initializer_list<std::initializer_list<float>> &values);
    static Matrix identity(int nrows);
    float *operator[](int row);
    const float *operator[](int row) const;
    Matrix operator*(const Matrix &m);
    bool operator==(const Matrix &other);
    std::string to_string();
    Matrix copy_matrix();
    Matrix submatrix(int row, int col);
    float determinant();
    float cofactor(int row, int col);
    Matrix inverse();
    Matrix transpose();
};

#endif // _MATRIX_HPP
