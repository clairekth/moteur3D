#ifndef _VECTOR3F_H_
#define _VECTOR3F_H_

#include <math.h>
#include <string>

struct Vector3f
{
    float x, y, z;

    Vector3f(float x, float y, float z) : x(x), y(y), z(z){};

    Vector3f operator-(const Vector3f &A) const
    {
        return Vector3f(x - A.x, y - A.y, z - A.z);
    }

    Vector3f operator+(const Vector3f &A) const
    {
        return Vector3f(x + A.x, y + A.y, z + A.z);
    }

    float operator*(const Vector3f &v) const
    {
        return x * v.x + y * v.y + z * v.z;
    }

    Vector3f operator*(float k) const
    {
        return Vector3f(x * k, y * k, z * k);
    }

    float operator[](int idx) const
    {
        return idx == 0 ? x : idx == 1 ? y
                                       : z;
    }

    float norm()
    {
        return sqrt(x * x + y * y + z * z);
    }
    std::string to_string()
    {
        return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z);
    }
};

Vector3f normalize(const Vector3f &v)
{
    float norm = sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
    if (norm == 0)
        return Vector3f(0, 0, 0);
    return Vector3f(v.x / norm, v.y / norm, v.z / norm);
};

#endif // _VECTOR3F_H_