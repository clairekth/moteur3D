#include "tgaimage.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
#include "Model.hpp"
#include "utils.hpp"
#include "Matrix.hpp"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 1024;
const int height = 1024;
const int depth = 250;
const Vector3f light_dir(0, 0, 1);
const Vector3f camera(0, 0, 3);

Matrix projection(4, 4);
Matrix viewport_matrix(4, 4);
Matrix model_view(4, 4);

Matrix move_camera(Vector3f center, Vector3f up, Vector3f eye)
{
	Vector3f z = (eye - center); // camera direction
	z.normalize();
	Vector3f x = cross_product(up, z); // camera right
	x.normalize();
	Vector3f y = cross_product(z, x); // camera up
	y.normalize();
	Matrix m = Matrix::identity(4);
	Matrix Tr = Matrix::identity(4);
	for (int i = 0; i < 3; i++)
	{
		m[0][i] = x[i];
		m[1][i] = y[i];
		m[2][i] = z[i];
		Tr[i][3] = -center[i];
	}
	return m * Tr;
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

void draw_fill_triangle(TGAImage &image, std::vector<Vector3f> pts, float *zbuffer, std::vector<Vector3f> normal_pts, std::vector<Vector3f> texture_pts, TGAImage &texture)
{

	// float x0 = (pts[0].x + 1) * 0.5 * width;
	// float y0 = (pts[0].y + 1) * 0.5 * height;

	// float x1 = (pts[1].x + 1) * 0.5 * width;
	// float y1 = (pts[1].y + 1) * 0.5 * height;

	// float x2 = (pts[2].x + 1) * 0.5 * width;
	// float y2 = (pts[2].y + 1) * 0.5 * height;

	// Vector3f A(x0, y0, 0);
	// Vector3f B(x1, y1, 0);
	// Vector3f C(x2, y2, 0);

	// int min_x = std::min(std::min(x0, x1), x2);
	// int min_y = std::min(std::min(y0, y1), y2);
	// int max_x = std::max(std::max(x0, x1), x2);
	// int max_y = std::max(std::max(y0, y1), y2);

	// Texture
	Vector3f texture_A(texture_pts[0].x, texture_pts[0].y, 0);
	Vector3f texture_B(texture_pts[1].x, texture_pts[1].y, 0);
	Vector3f texture_C(texture_pts[2].x, texture_pts[2].y, 0);

	Vector3f screen_coord_A = matrix2vector(viewport_matrix * projection * model_view * vector2matrix(pts[0]));
	Vector3f screen_coord_B = matrix2vector(viewport_matrix * projection * model_view * vector2matrix(pts[1]));
	Vector3f screen_coord_C = matrix2vector(viewport_matrix * projection * model_view * vector2matrix(pts[2]));

	int min_x = std::min(std::min(screen_coord_A.x, screen_coord_B.x), screen_coord_C.x);
	int min_y = std::min(std::min(screen_coord_A.y, screen_coord_B.y), screen_coord_C.y);
	int max_x = std::max(std::max(screen_coord_A.x, screen_coord_B.x), screen_coord_C.x);
	int max_y = std::max(std::max(screen_coord_A.y, screen_coord_B.y), screen_coord_C.y);

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			Vector3f P(x, y, 0);
			Vector3f barycenter = barycentric(screen_coord_A, screen_coord_B, screen_coord_C, P);
			// Vector3f barycenter = barycentric(A, B, C, P);
			if (is_inside(barycenter))
			{
				P.z = pts[0].z * barycenter.x + pts[1].z * barycenter.y + pts[2].z * barycenter.z;

				// Vector3f normal = cross_product(pts[1] - pts[0], pts[2] - pts[0]);
				// Vector3f normal = normal_pts[0] * barycenter.x + normal_pts[1] * barycenter.y + normal_pts[2] * barycenter.z;
				Vector3f normal = cross_product(screen_coord_B - screen_coord_A, screen_coord_C - screen_coord_A);
				normal.normalize();
				float intensity = std::max(0.0f, dot_product(normal, light_dir));

				if (zbuffer[x + y * width] < P.z)
				{
					float u = texture_A.x * barycenter.x + texture_B.x * barycenter.y + texture_C.x * barycenter.z;
					float v = texture_A.y * barycenter.x + texture_B.y * barycenter.y + texture_C.y * barycenter.z;
					// TGAColor color = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
					// TGAColor color = TGAColor(intensity * 255, intensity * 255, intensity * 255, 255);
					TGAColor color = texture.get(u * texture.get_width(), v * texture.get_height());
					color = TGAColor(color.r * intensity, color.g * intensity, color.b * intensity, 255);
					zbuffer[x + y * width] = P.z;
					image.set(x, y, color);
				}
			}
		}
	}
}

void draw_all_triangles(Model &model, TGAImage &image)
{
	int nb_triangles = model.get_nb_triangles();

	float zbuffer[width * height];
	for (int j = 0; j < width * height; j++)
	{
		zbuffer[j] = std::numeric_limits<float>::lowest();
	}

	for (int i = 0; i < nb_triangles; i++)
	{
		std::vector<Vector3f> pts = model.get_vertex_triangle(i);
		std::vector<Vector3f> normal_pts = model.get_normal_triangle(i);
		std::vector<Vector3f> texture_pts = model.get_texture_triangle(i);

		TGAImage texture = model.get_texture_diffuse();
		draw_fill_triangle(image, pts, zbuffer, normal_pts, texture_pts, texture);
	}
}

int main(int argc, char **argv)
{
	TGAImage image(width, height, TGAImage::RGB);
	Model m = Model("obj/african_head/african_head.obj");
	projection = Matrix::identity(4);
	viewport_matrix = viewport(width / 8, height / 8, width * 3 / 4, height * 3 / 4);
	// viewport_matrix = viewport(0, 0, width, height);
	// projection[3][2] = -1.f / camera.z;
	Vector3f eye(1, 1, 3);
	Vector3f center(0, 0, 0);
	Vector3f up(0, 1, 0);
	projection[3][2] = -1.f / (eye - center).norm();
	model_view = move_camera(center, up, eye);

	draw_all_triangles(m, image);
	image.flip_vertically();
	image.write_tga_file("output.tga");

	return 0;
}
