#include "tgaimage.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>
#include "model.h"

const TGAColor white = TGAColor(255, 255, 255, 255);
const TGAColor red = TGAColor(255, 0, 0, 255);
const int width = 800;
const int height = 800;

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

void draw_vertex(Model model, TGAImage &image, TGAColor color)
{
	int nb_vertex_draw = 0;
	int nb_vertex = model.vertex.size();
	for (int i = 0; i < nb_vertex; i++)
	{
		image.set(model.vertex[i].x, model.vertex[i].y, color);
		nb_vertex_draw++;
	}

	std::cout << "nb_vertex_draw: " << nb_vertex_draw << std::endl;
}

void draw_triangle(Model model, TGAImage &image, TGAColor color)
{
	int nb_triangles = model.facettes.size();
	int nb_triangles_draw = 0;

	for (int i = 0; i < nb_triangles; i++)
	{
		int x0 = model.vertex[model.facettes[i].ip0].x;
		int y0 = model.vertex[model.facettes[i].ip0].y;
		int x1 = model.vertex[model.facettes[i].ip1].x;
		int y1 = model.vertex[model.facettes[i].ip1].y;
		int x2 = model.vertex[model.facettes[i].ip2].x;
		int y2 = model.vertex[model.facettes[i].ip2].y;

		line(x0, y0, x1, y1, image, color);
		line(x1, y1, x2, y2, image, color);
		line(x2, y2, x0, y0, image, color);

		nb_triangles_draw++;
	}

	std::cout << "nb_triangles_draw: " << nb_triangles_draw << std::endl;
}

int main(int argc, char **argv)
{
	TGAImage image(width, height, TGAImage::RGB);
	Model m = Model("obj/african_head/african_head.obj", width, height);
	draw_vertex(m, image, white);
	draw_triangle(m, image, white);
	image.flip_vertically();
	image.write_tga_file("output.tga");
	return 0;
}
