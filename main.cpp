#include "Model.hpp"
#include "GL.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const int width = 1024;
const int height = 1024;
const int depth = 255;
const Vector3f light_dir = normalize(Vector3f(0, 0, 1));

void draw_triangle(std::vector<Vector3f> pts_vertex, std::vector<Vector3f> pts_normales, std::vector<Vector3f> pts_textures, float *zbuffer, TGAImage &image, TGAImage &texture)
{
    int min_x = std::min(std::min(pts_vertex[0].x, pts_vertex[1].x), pts_vertex[2].x);
    int min_y = std::min(std::min(pts_vertex[0].y, pts_vertex[1].y), pts_vertex[2].y);
    int max_x = std::max(std::max(pts_vertex[0].x, pts_vertex[1].x), pts_vertex[2].x);
    int max_y = std::max(std::max(pts_vertex[0].y, pts_vertex[1].y), pts_vertex[2].y);

    for (int x = min_x; x <= max_x; x++)
    {
        for (int y = min_y; y <= max_y; y++)
        {
            Vector3f P(x, y, 0);
            Vector3f barycenter = barycentric(pts_vertex[0], pts_vertex[1], pts_vertex[2], P);

            if (is_inside(barycenter))
            {
                P.z = pts_vertex[0].z * barycenter.x + pts_vertex[1].z * barycenter.y + pts_vertex[2].z * barycenter.z;

                Vector3f normal = normalize(pts_normales[0] * barycenter.x + pts_normales[1] * barycenter.y + pts_normales[2] * barycenter.z);
                float intensity = dot_product(normal, light_dir);
                if (intensity < 0)
                    continue;

                int z_index = x + y * width;
                if (z_index < 0 || z_index >= width * height)
                    continue;

                if (zbuffer[z_index] < P.z)
                {
                    zbuffer[z_index] = P.z;

                    // TGAColor color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255);

                    float u = pts_textures[0].x * barycenter.x + pts_textures[1].x * barycenter.y + pts_textures[2].x * barycenter.z;
                    float v = pts_textures[0].y * barycenter.x + pts_textures[1].y * barycenter.y + pts_textures[2].y * barycenter.z;
                    TGAColor color = texture.get(u * texture.get_width(), v * texture.get_height());
                    color = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, 255);

                    image.set(x, y, color);
                }
            }
        }
    }
}

int main(int argc, char **argv)
{
    TGAImage image(width, height, TGAImage::RGB);
    Model m = Model("obj/african_head/african_head.obj");
    TGAImage texture = m.get_texture_diffuse();

    // draw_all_vertex(m.get_vertex(), image, green, width, height);

    float zbuffer[width * height];
    for (int j = 0; j < width * height; j++)
    {
        zbuffer[j] = std::numeric_limits<float>::lowest();
    }

    Matrix rotation = rotation_matrix(Vector3f(0, 0, 0));
    Matrix translation = translation_matrix(Vector3f(0, 0, 0));
    Matrix scale = scale_matrix(Vector3f(1, 1, 1));
    Matrix M = translation * rotation * scale;

    Vector3f eye(1, 1, 3);
    Vector3f center(0, 0, 0);
    Vector3f up(0, 1, 0);

    Matrix projection_matrix = projection(eye, center);
    Matrix viewport_matrix = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4, depth);
    Matrix model_view = lookAt(center, up, eye);

    for (int i = 0; i < m.get_nb_triangles(); i++)
    {
        std::vector<Vector3f> vertex = m.get_vertex_triangle(i);
        std::vector<Vector3f> normales = m.get_normal_triangle(i);
        std::vector<Vector3f> pts_textures = m.get_texture_triangle(i);

        Vector3f A_vertex = matrix2vector(viewport_matrix * projection_matrix * model_view * M * vector2matrix(vertex[0]));
        Vector3f B_vertex = matrix2vector(viewport_matrix * projection_matrix * model_view * M * vector2matrix(vertex[1]));
        Vector3f C_vertex = matrix2vector(viewport_matrix * projection_matrix * model_view * M * vector2matrix(vertex[2]));

        Vector3f A_normal = matrix2vector(model_view * M * vector2matrix(normales[0]));
        Vector3f B_normal = matrix2vector(model_view * M * vector2matrix(normales[1]));
        Vector3f C_normal = matrix2vector(model_view * M * vector2matrix(normales[2]));

        std::vector<Vector3f> pts_vertex = {A_vertex, B_vertex, C_vertex};
        std::vector<Vector3f> pts_normales = {A_normal, B_normal, C_normal};

        draw_triangle(pts_vertex, pts_normales, pts_textures, zbuffer, image, texture);
    }

    image.flip_vertically();
    image.write_tga_file("output.tga");

    return 0;
}
