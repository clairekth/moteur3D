#ifndef _UTILS_H_
#define _UTILS_H_

#include "Matrix.hpp"

void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color)
{
    bool steep = false;
    if (std::abs(x0 - x1) < std::abs(y0 - y1))
    {
        std::swap(x0, y0);
        std::swap(x1, y1);
        steep = true;
    }
    if (x0 > x1)
    {
        std::swap(x0, x1);
        std::swap(y0, y1);
    }
    int dx = x1 - x0;
    int dy = y1 - y0;
    int derror2 = std::abs(dy) * 2;
    int error2 = 0;
    int y = y0;
    for (int x = x0; x <= x1; x++)
    {
        if (steep)
        {
            image.set(y, x, color);
        }
        else
        {
            image.set(x, y, color);
        }
        error2 += derror2;
        if (error2 > dx)
        {
            y += (y1 > y0 ? 1 : -1);
            error2 -= dx * 2;
        }
    }
}

void draw_vertex(Model &model, TGAImage &image, TGAColor color)
{
    // 	int nb_vertex = model.vertex.size();
    // 	for (int i = 0; i < nb_vertex; i++)
    // 	{
    // 		image.set(model.vertex[i].x, model.vertex[i].y, color);
    // 	}
}

void draw_line_triangle(Vector3f A, Vector3f B, Vector3f C, TGAImage &image, TGAColor color)
{

    line(A.x, A.y, B.x, B.y, image, color);
    line(A.x, A.y, C.x, C.y, image, color);
    line(B.x, B.y, C.x, C.y, image, color);
}

Vector3f barycentric(Vector3f A, Vector3f B, Vector3f C, Vector3f P)
{
    // Calcul triangle areas
    float ABC = ((B.x - A.x) * (C.y - A.y) - (B.y - A.y) * (C.x - A.x));
    float PBC = ((B.x - P.x) * (C.y - P.y) - (B.y - P.y) * (C.x - P.x));
    float PCA = ((C.x - P.x) * (A.y - P.y) - (C.y - P.y) * (A.x - P.x));
    float PAB = ((A.x - P.x) * (B.y - P.y) - (A.y - P.y) * (B.x - P.x));

    // Barycentric coordinates
    float alpha = PBC / ABC;
    float beta = PCA / ABC;
    float gamma = PAB / ABC;

    return Vector3f(alpha, beta, gamma);
}

bool is_inside(Vector3f barycenter)
{
    return barycenter.x >= 0 && barycenter.y >= 0 && barycenter.z >= 0 && barycenter.x <= 1 && barycenter.y <= 1 && barycenter.z <= 1;
}

Vector3f cross_product(Vector3f A, Vector3f B)
{
    return Vector3f(A.y * B.z - A.z * B.y, A.z * B.x - A.x * B.z, A.x * B.y - A.y * B.x);
}

float dot_product(Vector3f A, Vector3f B)
{
    return A.x * B.x + A.y * B.y + A.z * B.z;
}

Matrix vector2matrix(Vector3f v)
{
    Matrix m(4, 1);
    m[0][0] = v.x;
    m[1][0] = v.y;
    m[2][0] = v.z;
    m[3][0] = 1.0f;
    return m;
}

Vector3f matrix2vector(Matrix m)
{
    return Vector3f(m[0][0] / m[3][0], m[1][0] / m[3][0], m[2][0] / m[3][0]);
}

#endif // _UTILS_H_
