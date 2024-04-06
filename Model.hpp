#ifndef MODEL_H
#define MODEL_H

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

public:
    Model(const char *filename);
    ~Model();
    void load_texture(const char *filename);
    int get_nb_triangles();
    std::vector<Vector3f> get_vertex_triangle(const int i);
    std::vector<Vector3f> get_texture_triangle(const int i);
    std::vector<Vector3f> get_normal_triangle(const int i);
    TGAImage get_texture_diffuse();
    void draw_all_vertex(TGAImage &image, TGAColor color, int width, int height);
};

#endif // MODEL_H