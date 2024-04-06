#ifndef VECTOR3F_H
#define VECTOR3F_H

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

    void normalize()
    {
        float norm = sqrt(x * x + y * y + z * z);
        if (norm == 0)
        {
            x = 0.f;
            y = 0.f;
            z = 0.f;
        }
        else
        {
            x /= norm;
            y /= norm;
            z /= norm;
        }
    };

    std::string to_string()
    {
        return "x: " + std::to_string(x) + " y: " + std::to_string(y) + " z: " + std::to_string(z);
    }
};

#endif // VECTOR3F_H