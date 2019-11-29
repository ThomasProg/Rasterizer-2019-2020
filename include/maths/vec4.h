#ifndef _VEC4_H_
#define _VEC4_H_

#include <iostream>

struct Vec3;
class  Mat4;

struct Vec4
{
public:
    float x, y, z, w;

    Vec4(const Vec4& vec);
    Vec4(float x, float y, float z, float w);
    Vec4(const Vec3& vec3, float w = 1.f);

    Vec4 operator*(const Mat4&) const = delete; //line convention
    Vec4 operator-(const Vec4&) const;
    float& operator[](unsigned int index);
    float operator[](unsigned int index) const;

    bool  isInsideWSizedCube();
    float GetMagnitude() const;
    void  Normalize();
    void  Homogenize();
    Vec3  getHomogenizedVec() const;
};

Vec4 operator+(const Vec4&, const Vec4&);
Vec4 operator*(const Vec4&, float);
std::ostream& operator<<(std::ostream& stream, const Vec4& vector);

#endif