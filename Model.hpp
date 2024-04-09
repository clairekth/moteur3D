#ifndef _MODEL_H_
#define _MODEL_H_

#include "Utils.hpp"

class Model
{
protected:
    std::vector<Vector3f> vertex;
    std::vector<Triangle> triangles;
    std::vector<Triangle> texture_coordinates;
    std::vector<Vector3f> vn;
    std::vector<Vector3f> vt;
    TGAImage texture_diffuse;
    TGAImage nm;
    TGAImage specular_map;

public:
    Model(const char *filename);
    ~Model();
    int get_nb_triangles();
    std::vector<Vector3f> get_vertex();
    std::vector<Vector3f> get_vertex_triangle(const int i);
    std::vector<Vector3f> get_texture_triangle(const int i);
    std::vector<Vector3f> get_normal_triangle(const int i);
    TGAImage get_texture_diffuse();
    TGAImage get_nm();
    TGAImage get_specular_map();
};

#endif // _MODEL_H_