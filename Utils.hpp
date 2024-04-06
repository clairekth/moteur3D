#ifndef _UTILS_H_
#define _UTILS_H_

#include "Matrix.hpp"
#include "tgaimage.h"
#include "Vector3f.hpp"
#include <iostream>
#include <vector>

struct Triangle
{
    int ip0, ip1, ip2; // index of the points
    Triangle(int ip0, int ip1, int ip2) : ip0(ip0), ip1(ip1), ip2(ip2) {}
};

void draw_all_vertex(std::vector<Vector3f> vertex, TGAImage &image, TGAColor color, int width, int height);
void line(int x0, int y0, int x1, int y1, TGAImage &image, TGAColor color);

void draw_line_triangle(Vector3f A, Vector3f B, Vector3f C, TGAImage &image, TGAColor color);

Vector3f barycentric(Vector3f A, Vector3f B, Vector3f C, Vector3f P);

bool is_inside(Vector3f barycenter);

Vector3f cross_product(Vector3f A, Vector3f B);
float dot_product(Vector3f A, Vector3f B);

Matrix vector2matrix(Vector3f v);

Vector3f matrix2vector(Matrix m);
Vector3f world_to_screen(Vector3f point, int width, int height);
#endif // _UTILS_H_
