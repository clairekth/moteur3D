#include "tgaimage.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
#include "Model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 800;
const int height = 800;
const Vector3f light_dir(0, 0, -1);

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
	int nb_vertex = model.vertex.size();
	for (int i = 0; i < nb_vertex; i++)
	{
		image.set(model.vertex[i].x, model.vertex[i].y, color);
	}
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

void draw_fill_triangle(TGAImage &image, std::vector<Vector3f> pts, float *zbuffer, TGAColor color)
{
	float x0 = (pts[0].x + 1) * 0.5 * width;
	float y0 = (pts[0].y + 1) * 0.5 * height;

	float x1 = (pts[1].x + 1) * 0.5 * width;
	float y1 = (pts[1].y + 1) * 0.5 * height;

	float x2 = (pts[2].x + 1) * 0.5 * width;
	float y2 = (pts[2].y + 1) * 0.5 * height;

	Vector3f A(x0, y0, 0);
	Vector3f B(x1, y1, 0);
	Vector3f C(x2, y2, 0);

	int min_x = std::min(std::min(x0, x1), x2);
	int min_y = std::min(std::min(y0, y1), y2);
	int max_x = std::max(std::max(x0, x1), x2);
	int max_y = std::max(std::max(y0, y1), y2);

	for (int x = min_x; x <= max_x; x++)
	{
		for (int y = min_y; y <= max_y; y++)
		{
			Vector3f P(x, y, 0);
			Vector3f barycenter = barycentric(A, B, C, P);
			if (is_inside(barycenter))
			{
				P.z = pts[0].z * barycenter.x + pts[1].z * barycenter.y + pts[2].z * barycenter.z;
				if (zbuffer[x + y * width] < P.z)
				{
					zbuffer[x + y * width] = P.z;
					image.set(x, y, color);
				}
			}
		}
	}
}

void draw_all_triangles(Model &model, TGAImage &image)
{
	int nb_triangles = model.triangles.size();

	float *zbuffer = new float[width * height];
	for (int j = 0; j < width * height; j++)
	{
		zbuffer[j] = std::numeric_limits<float>::lowest();
	}

	for (int i = 0; i < nb_triangles; i++)
	{
		Vector3f world_A(model.vertex[model.triangles[i].ip0].x, model.vertex[model.triangles[i].ip0].y, model.vertex[model.triangles[i].ip0].z);
		Vector3f world_B(model.vertex[model.triangles[i].ip1].x, model.vertex[model.triangles[i].ip1].y, model.vertex[model.triangles[i].ip1].z);
		Vector3f world_C(model.vertex[model.triangles[i].ip2].x, model.vertex[model.triangles[i].ip2].y, model.vertex[model.triangles[i].ip2].z);

		std::vector<Vector3f> pts = {world_A, world_B, world_C};

		// Intensity
		Vector3f normal = cross_product(world_C - world_A, world_B - world_A);
		normal.normalize();
		float intensity = dot_product(normal, light_dir);

		if (intensity < 0)
			continue;

		// TGAColor color = TGAColor(rand() % 255, rand() % 255, rand() % 255, 255);
		TGAColor color = TGAColor(intensity * 255, intensity * 255, intensity * 255, 255);

		draw_fill_triangle(image, pts, zbuffer, color);
	}
}

int main(int argc, char **argv)
{
	TGAImage image(width, height, TGAImage::RGB);
	Model m = Model("obj/african_head/african_head.obj");
	draw_all_triangles(m, image);
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
