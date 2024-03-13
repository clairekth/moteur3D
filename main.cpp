#include "tgaimage.h"
#include <fstream>
#include <string.h>
#include <iostream>
#include <vector>

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

std::vector<std::vector<float>> parser_obj(const char *filename)
{
	std::ifstream in;
	in.open(filename, std::ifstream::in);
	std::vector<std::vector<float>> vertex;

	if (!in.is_open())
	{
		std::cerr << "Cannot open " << filename << std::endl;
		return vertex;
	}
	std::string line;
	float x, y, z;
	int nb_vertex = 0;

	while (!in.eof())
	{
		while (std::getline(in, line))
		{
			if (line.c_str()[0] == 'v' && line.c_str()[1] == ' ')
			{
				sscanf(line.c_str(), "v %f %f %f", &x, &y, &z);
				std::cout
					<< "x : " << x << " y : " << y << " z :" << z << std::endl;
				nb_vertex++;
				vertex.push_back({x, y, z});
			}
		}
	}
	std::cout << "nb_vertex : " << nb_vertex << std::endl;
	std::cout << "vertex.size() : " << vertex.size() << std::endl;
	in.close();

	return vertex;
}

void draw_vertex(std::vector<std::vector<float>> vertex, TGAImage &image, TGAColor color)
{
	int nb_vertex_draw = 0;
	int nb_vertex = vertex.size();
	for (int i = 0; i < nb_vertex; i++)
	{
		// Remap the vertex to the image : -1 = 0, 1 = image.width or image.height
		// add + 1 so the vertex is between 0 and 2
		// multiply by image.width or image.height
		// and divide by 2 to get the right position ( 2 = image width or height ; x, y = ? so ? = x * image.width or height / 2)
		int x = (vertex[i][0] + 1) * image.get_width() / 2;
		int y = (vertex[i][1] + 1) * image.get_height() / 2;
		image.set(x, y, color);
		nb_vertex_draw++;
	}

	std::cout << "nb_vertex_draw: " << nb_vertex_draw << std::endl;
}

int main(int argc, char **argv)
{
	TGAImage image(width, height, TGAImage::RGB);
	std::vector<std::vector<float>> v = parser_obj("obj/african_head/african_head.obj");
	draw_vertex(v, image, white);
	image.flip_vertically(); // i want to have the origin at the left bottom corner of the image
	image.write_tga_file("output.tga");
	return 0;
}
