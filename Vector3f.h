#ifndef VECTOR3F_H
#define VECTOR3F_H

#include <math.h>

struct Vector3f
{
    float x, y, z;

    Vector3f(float x, float y, float z) : x(x), y(y), z(z){};

    friend Vector3f operator-(const Vector3f &A, const Vector3f &B)
    {
        return Vector3f(A.x - B.x, A.y - B.y, A.z - B.z);
    }

    void normalize()
    {
        float norm = sqrt(x * x + y * y + z * z);
        x /= norm;
        y /= norm;
        z /= norm;
    };
};

#endif // VECTOR3F_H