#ifndef _GL_H_
#define _GL_H_

#include "Vector3f.hpp"
#include "Utils.hpp"
#include "Matrix.hpp"

Matrix lookAt(Vector3f center, Vector3f up, Vector3f eye)
{
    Vector3f z_axis = normalize(eye - center);                  // camera direction
    Vector3f x_axis = normalize(cross_product(up, z_axis));     // camera right
    Vector3f y_axis = normalize(cross_product(z_axis, x_axis)); // camera up

    // Orientation matrix
    Matrix O({{x_axis.x, x_axis.y, x_axis.z, 0},
              {y_axis.x, y_axis.y, y_axis.z, 0},
              {z_axis.x, z_axis.y, z_axis.z, 0},
              {0, 0, 0, 1}});

    // Translation matrix
    Matrix T({{1, 0, 0, -center.x},
              {0, 1, 0, -center.y},
              {0, 0, 1, -center.z},
              {0, 0, 0, 1}});

    return O * T;
}

Matrix viewport(int x, int y, int w, int h, int depth)
{
    Matrix viewport_matrix = Matrix::identity(4);
    viewport_matrix[0][3] = x + w / 2.f;
    viewport_matrix[1][3] = y + h / 2.f;
    viewport_matrix[2][3] = depth / 2.f;

    viewport_matrix[0][0] = w / 2.f;
    viewport_matrix[1][1] = h / 2.f;
    viewport_matrix[2][2] = depth / 2.f;

    return viewport_matrix;
}

Matrix projection(Vector3f eye, Vector3f center)
{
    Matrix projection_matrix = Matrix::identity(4);
    projection_matrix[3][2] = -1 / (eye - center).norm();
    return projection_matrix;
}

Matrix rotation_matrix(Vector3f angles)
{
    angles = angles * (M_PI / 180);

    Matrix rotation_x = Matrix::identity(4);
    rotation_x[1][1] = cos(angles.x);
    rotation_x[1][2] = -sin(angles.x);
    rotation_x[2][1] = sin(angles.x);
    rotation_x[2][2] = cos(angles.x);

    Matrix rotation_y = Matrix::identity(4);
    rotation_y[0][0] = cos(angles.y);
    rotation_y[0][2] = sin(angles.y);
    rotation_y[2][0] = -sin(angles.y);
    rotation_y[2][2] = cos(angles.y);

    Matrix rotation_z = Matrix::identity(4);
    rotation_z[0][0] = cos(angles.z);
    rotation_z[0][1] = -sin(angles.z);
    rotation_z[1][0] = sin(angles.z);
    rotation_z[1][1] = cos(angles.z);

    return rotation_z * rotation_y * rotation_x;
}

Matrix translation_matrix(Vector3f translation)
{
    Matrix translation_matrix = Matrix::identity(4);
    translation_matrix[0][3] = translation.x;
    translation_matrix[1][3] = translation.y;
    translation_matrix[2][3] = translation.z;
    return translation_matrix;
}

Matrix scale_matrix(Vector3f scale)
{
    Matrix scale_matrix = Matrix::identity(4);
    scale_matrix[0][0] = scale.x;
    scale_matrix[1][1] = scale.y;
    scale_matrix[2][2] = scale.z;
    return scale_matrix;
}

#endif // _GL_H_