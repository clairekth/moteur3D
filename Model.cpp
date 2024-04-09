#include "Model.hpp"

Model::Model(const char *filename)
{
    std::ifstream in;
    in.open(filename, std::ifstream::in);

    if (!in.is_open())
    {
        std::cerr << "Cannot open " << filename << std::endl;
        return;
    }

    std::string line;
    while (!in.eof())
    {
        while (std::getline(in, line))
        {
            if (line.c_str()[0] == 'v' && line.c_str()[1] == ' ')
            {
                float a, b, c;
                sscanf(line.c_str(), "v %f %f %f", &a, &b, &c);
                vertex.push_back({a, b, c});
            }
            else if (line.c_str()[0] == 'f' && line.c_str()[1] == ' ')
            {
                int a1, b1, c1, a2, b2, c2, a3, b3, c3;
                sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d", &a1, &b1, &c1, &a2, &b2, &c2, &a3, &b3, &c3);
                triangles.push_back({a1 - 1, a2 - 1, a3 - 1});
                texture_coordinates.push_back({b1 - 1, b2 - 1, b3 - 1});
            }
            else if (line.c_str()[0] == 'v' && line.c_str()[1] == 'n')
            {
                float a, b, c;
                sscanf(line.c_str(), "vn %f %f %f", &a, &b, &c);
                vn.push_back({a, b, c});
            }
            else if (line.c_str()[0] == 'v' && line.c_str()[1] == 't')
            {
                float a, b;
                sscanf(line.c_str(), "vt %f %f", &a, &b);
                vt.push_back({a, b, 0});
            }
        }
    }

    in.close();
    texture_diffuse.read_tga_file("obj/african_head/african_head_diffuse.tga");
    texture_diffuse.flip_vertically();
    nm.read_tga_file("obj/african_head/african_head_nm.tga");
    nm.flip_vertically();
    specular_map.read_tga_file("obj/african_head/african_head_spec.tga");
    specular_map.flip_vertically();
};

Model::~Model() {}

int Model::get_nb_triangles()
{
    return triangles.size();
}

std::vector<Vector3f> Model::get_vertex_triangle(const int i)
{
    Vector3f world_A(vertex[triangles[i].ip0].x, vertex[triangles[i].ip0].y, vertex[triangles[i].ip0].z);
    Vector3f world_B(vertex[triangles[i].ip1].x, vertex[triangles[i].ip1].y, vertex[triangles[i].ip1].z);
    Vector3f world_C(vertex[triangles[i].ip2].x, vertex[triangles[i].ip2].y, vertex[triangles[i].ip2].z);

    std::vector<Vector3f> pts = {world_A, world_B, world_C};

    return pts;
}

std::vector<Vector3f> Model::get_texture_triangle(const int i)
{
    Vector3f texture_A(vt[texture_coordinates[i].ip0].x, vt[texture_coordinates[i].ip0].y, vt[texture_coordinates[i].ip0].z);
    Vector3f texture_B(vt[texture_coordinates[i].ip1].x, vt[texture_coordinates[i].ip1].y, vt[texture_coordinates[i].ip1].z);
    Vector3f texture_C(vt[texture_coordinates[i].ip2].x, vt[texture_coordinates[i].ip2].y, vt[texture_coordinates[i].ip2].z);

    std::vector<Vector3f> texture_pts = {texture_A, texture_B, texture_C};

    return texture_pts;
}

std::vector<Vector3f> Model::get_normal_triangle(const int i)
{
    Vector3f normal_A(vn[triangles[i].ip0].x, vn[triangles[i].ip0].y, vn[triangles[i].ip0].z);
    Vector3f normal_B(vn[triangles[i].ip1].x, vn[triangles[i].ip1].y, vn[triangles[i].ip1].z);
    Vector3f normal_C(vn[triangles[i].ip2].x, vn[triangles[i].ip2].y, vn[triangles[i].ip2].z);

    std::vector<Vector3f> normal_pts = {normal_A, normal_B, normal_C};

    return normal_pts;
}

TGAImage Model::get_texture_diffuse()
{
    return texture_diffuse;
}

TGAImage Model::get_nm()
{
    return nm;
}

TGAImage Model::get_specular_map()
{
    return specular_map;
}

std::vector<Vector3f> Model::get_vertex()
{
    return vertex;
}
