#include <cmath>

#include "vec3.h"
#include "vec4.h"

Vec3::Vec3(float x, float y, float z)
    : x(x), 
      y(y),
      z(z)
{

}

Vec3::Vec3(const Vec4& copiedV4)
    : x(copiedV4.x),
      y(copiedV4.y),
      z(copiedV4.z)
{

}

float Vec3::GetMagnitude() const
{
    return sqrt(pow(x, 2) + pow(y, 2) + pow(z, 2));
}

void  Vec3::Normalize()
{
    float magnitude = GetMagnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

Vec3 operator+(const Vec3& a, const Vec3& b)
{
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}

Vec3 operator*(const Vec3& vec, float f)
{
    return Vec3(vec.x * f, vec.y * f, vec.z * f);
}

Vec3 operator*(float f, const Vec3& vec)
{
    return Vec3(vec.x * f, vec.y * f, vec.z * f);
}

std::ostream& operator<<(std::ostream& stream, const Vec3& vector)
{
    stream << "x : " << vector.x << " | y : " << vector.y << " | z : " << vector.z << std::endl;
    return stream;
}