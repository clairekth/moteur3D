#include "Model.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const TGAColor green = TGAColor(0, 255, 0, 255);
const int width = 1024;
const int height = 1024;
const int depth = 255;

Matrix projection(4, 4);
Matrix viewport_matrix(4, 4);
Matrix model_view(4, 4);
Matrix M(4, 4);

Matrix lookAt(Vector3f center, Vector3f up, Vector3f eye)
{
    Vector3f z_axis = (eye - center); // camera direction
    z_axis.normalize();
    Vector3f x_axis = cross_product(up, z_axis); // camera right
    x_axis.normalize();
    Vector3f y_axis = cross_product(z_axis, x_axis); // camera up
    y_axis.normalize();

    Matrix O({{x_axis.x, x_axis.y, x_axis.z, 0},
              {y_axis.x, y_axis.y, y_axis.z, 0},
              {z_axis.x, z_axis.y, z_axis.z, 0},
              {0, 0, 0, 1}});

    Matrix T({{1, 0, 0, -center.x},
              {0, 1, 0, -center.y},
              {0, 0, 1, -center.z},
              {0, 0, 0, 1}});

    return O * T;
}

Matrix viewport(int x, int y, int w, int h)
{
    Matrix m = Matrix::identity(4);
    m[0][3] = x + w / 2.f;
    m[1][3] = y + h / 2.f;
    m[2][3] = depth / 2.f;

    m[0][0] = w / 2.f;
    m[1][1] = h / 2.f;
    m[2][2] = depth / 2.f;
    return m;
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

int main(int argc, char **argv)
{
    TGAImage image(width, height, TGAImage::RGB);
    Model m = Model("obj/african_head/african_head.obj");
    TGAImage texture = m.get_texture_diffuse();

    m.draw_all_vertex(image, green, width, height);

    float zbuffer[width * height];
    for (int j = 0; j < width * height; j++)
    {
        zbuffer[j] = std::numeric_limits<float>::lowest();
    }

    Matrix rotation = rotation_matrix(Vector3f(0, 0, 0));

    // Translation
    Matrix translation = Matrix::identity(4);
    translation[0][3] = 0;
    translation[1][3] = 0;
    translation[2][3] = 0;

    // Scale
    Matrix scale = Matrix::identity(4);
    scale[0][0] = 1;
    scale[1][1] = 1;
    scale[2][2] = 1;

    // M = translation * scale * rotation;
    M = translation * rotation * scale;

    projection = Matrix::identity(4);
    viewport_matrix = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);

    Vector3f eye(0, 0, 10);
    Vector3f center(0, 0, 0);

    Vector3f up(0, 1, 0);
    projection[3][2] = -1.f / (eye - center).norm();
    model_view = lookAt(center, up, eye);

    // std::cout << "model_view: \n"
    //           << model_view.to_string() << "\n";
    // std::cout << "projection: \n"
    //           << projection.to_string() << "\n";
    // std::cout << "viewport_matrix: \n " << viewport_matrix.to_string() << "\n";

    // for (int i = 0; i < m.get_nb_triangles(); i++)
    // {
    //     std::vector<Vector3f> vertex = m.get_vertex_triangle(i);
    //     std::vector<Vector3f> normales = m.get_normal_triangle(i);
    //     std::vector<Vector3f> textures = m.get_texture_triangle(i);

    //     Vector3f A_vertex = (vertex[0]);
    //     Vector3f B_vertex = (vertex[1]);
    //     Vector3f C_vertex = (vertex[2]);

    //     Vector3f A_normal = (normales[0]);
    //     Vector3f B_normal = (normales[1]);
    //     Vector3f C_normal = (normales[2]);

    //     Vector3f A_texture = (textures[0]);
    //     Vector3f B_texture = (textures[1]);
    //     Vector3f C_texture = (textures[2]);

    //     // A = matrix2vector(viewport_matrix * projection * model_view * vector2matrix(A));
    //     // B = matrix2vector(viewport_matrix * projection * model_view * vector2matrix(B));
    //     // C = matrix2vector(viewport_matrix * projection * model_view * vector2matrix(C));

    //     // std::cout << "A: " << A.to_string() << "\n";
    //     // std::cout << "B: " << B.to_string() << "\n";
    //     // std::cout << "C: " << C.to_string() << "\n";

    //     A_vertex = matrix2vector(viewport_matrix * projection * model_view * M * vector2matrix(A_vertex));
    //     B_vertex = matrix2vector(viewport_matrix * projection * model_view * M * vector2matrix(B_vertex));
    //     C_vertex = matrix2vector(viewport_matrix * projection * model_view * M * vector2matrix(C_vertex));

    //     // A_vertex = matrix2vector(model_view * M * vector2matrix(A_vertex));
    //     // B_vertex = matrix2vector(model_view * M * vector2matrix(B_vertex));
    //     // C_vertex = matrix2vector(model_view * M * vector2matrix(C_vertex));

    //     // std::cout << "A_vertex: " << A_vertex.to_string() << "\n";
    //     // std::cout << "B_vertex: " << B_vertex.to_string() << "\n";
    //     // std::cout << "C_vertex: " << C_vertex.to_string() << "\n";

    //     A_normal = matrix2vector(model_view * M * vector2matrix(A_normal));
    //     B_normal = matrix2vector(model_view * M * vector2matrix(B_normal));
    //     C_normal = matrix2vector(model_view * M * vector2matrix(C_normal));

    //     // A_vertex = world_to_screen(A_vertex);
    //     // B_vertex = world_to_screen(B_vertex);
    //     // C_vertex = world_to_screen(C_vertex);

    //     // std::cout << "--A_vertex: " << A_vertex.to_string() << "\n";
    //     // std::cout << "--B_vertex: " << B_vertex.to_string() << "\n";
    //     // std::cout << "--C_vertex: " << C_vertex.to_string() << "\n";

    //     int min_x = std::min(std::min(A_vertex.x, B_vertex.x), C_vertex.x);
    //     int min_y = std::min(std::min(A_vertex.y, B_vertex.y), C_vertex.y);
    //     int max_x = std::max(std::max(A_vertex.x, B_vertex.x), C_vertex.x);
    //     int max_y = std::max(std::max(A_vertex.y, B_vertex.y), C_vertex.y);

    //     for (int x = min_x; x <= max_x; x++)
    //     {
    //         for (int y = min_y; y <= max_y; y++)
    //         {
    //             Vector3f P(x, y, 0);
    //             Vector3f barycenter = barycentric(A_vertex, B_vertex, C_vertex, P);
    //             // std::cout << P.to_string() << "\n";
    //             // std::cout << "barycenter: " << barycenter.to_string() << "\n";
    //             if (is_inside(barycenter))
    //             {
    //                 P.z = A_vertex.z * barycenter.x + B_vertex.z * barycenter.y + C_vertex.z * barycenter.z;

    //                 // Vector3f normal = cross_product(B_vertex - A_vertex, C_vertex - A_vertex);
    //                 // Vector3f normal = cross_product(B_normal - A_normal, C_normal - A_normal);
    //                 Vector3f normal = A_normal * barycenter.x + B_normal * barycenter.y + C_normal * barycenter.z;
    //                 normal.normalize();
    //                 Vector3f light_dir(0, 0, 1);
    //                 light_dir.normalize();
    //                 // float intensity = std::max(0.0f, dot_product(normal, light_dir));

    //                 float intensity = dot_product(normal, light_dir);
    //                 if (intensity < 0)
    //                     continue;

    //                 // std::cout << "P.z: " << P.z << "\n";
    //                 // std::cout << "zbuffer[x + y * width]: " << x + y * width << "\n";
    //                 float z_index = x + y * width;
    //                 if (z_index < 0 || z_index >= width * height)
    //                 {
    //                     continue;
    //                 }
    //                 if (zbuffer[x + y * width] < P.z)
    //                 {
    //                     float u = A_texture.x * barycenter.x + B_texture.x * barycenter.y + C_texture.x * barycenter.z;
    //                     float v = A_texture.y * barycenter.x + B_texture.y * barycenter.y + C_texture.y * barycenter.z;

    //                     TGAColor color = texture.get(u * texture.get_width(), v * texture.get_height());
    //                     color = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, 255);
    //                     zbuffer[x + y * width] = P.z;
    //                     // TGAColor color = TGAColor(255 * intensity, 255 * intensity, 255 * intensity, 255);
    //                     image.set(x, y, color);
    //                 }
    //             }
    //         }
    //     }
    // }
    image.flip_vertically();
    char filename[100];
    sprintf(filename, "output.tga");
    image.write_tga_file(filename);

    return 0;
}
