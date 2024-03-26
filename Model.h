#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include <string.h>
#include <iostream>
#include <fstream>
#include "Vector3f.h"

struct Triangle
{
    int ip0, ip1, ip2; // index of the points
    Triangle(int ip0, int ip1, int ip2) : ip0(ip0), ip1(ip1), ip2(ip2) {}
};

struct Model
{
    std::vector<Vector3f> vertex;
    std::vector<Triangle> triangles;

    Model(const char *filename)
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
                }
            }
        }
    };
};

#endif // MODEL_H