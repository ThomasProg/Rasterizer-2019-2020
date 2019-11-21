#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>

struct Vec4;

struct Vec3
{
public:
    float x, y, z;

    Vec3(float x, float y, float z);
    Vec3(const Vec4&);

    float GetMagnitude() const;
    void  Normalize();
   
};

Vec3 operator+(const Vec3&, const Vec3&);

Vec3 operator*(const Vec3&, float);
Vec3 operator*(float, const Vec3&);
std::ostream& operator<<(std::ostream& stream, const Vec3& vector);

#endif