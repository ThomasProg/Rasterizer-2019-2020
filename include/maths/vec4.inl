#include <cmath>
#include <cassert>

#include "vec4.h"
#include "vec3.h"

__inline
Vec4::Vec4(const Vec4& vec)
    : x(vec.x), 
      y(vec.y),
      z(vec.z),
      w(vec.w)
{
    
}

__inline
Vec4::Vec4(float x, float y, float z, float w)
    : x(x), 
      y(y),
      z(z),
      w(w)
{

}

__inline
Vec4::Vec4(const Vec3& vec3, float w)
{
    x = vec3.x;
    y = vec3.y;
    z = vec3.z;
    this->w = w;
}

__inline
float Vec4::GetMagnitude() const
{
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
}

__inline
void Vec4::Normalize()
{
    float magnitude = GetMagnitude();
    x /= magnitude;
    y /= magnitude;
    z /= magnitude;
}

__inline
void Vec4::Homogenize()
{
    if (w != 0)
    {
        x /= w;
        y /= w;
        z /= w;
        w /= w;
    }
}

__inline
Vec3 Vec4::getHomogenizedVec() const
{
    // assert(w != 0);
    if (w != 0)
        return Vec3(x / w, y / w, z / w);
    else 
        return Vec3(0, 0, 0);
}

__inline
bool Vec4::isInsideWSizedCube()
{
    return !((x > w || z > w || y > w) || (x < -w || z < -w || y < -w));
}

__inline
float& Vec4::operator[](unsigned int index)
{
    assert(index < 4);
    switch (index)
    {
    case 0:
        return x;
        break;
    case 1:
        return y;
        break;
    
    case 2:
        return z;
        break;

    default: //the only left index is 3
        return w;
        break;
    }
}

__inline
float Vec4::operator[](unsigned int index) const
{
    assert(index < 4);

    return arr[index];
}

__inline
Vec4 operator+(const Vec4& a, const Vec4& b)
{
    return Vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}

__inline
Vec4 Vec4::operator-(const Vec4& v) const
{
    return Vec4(x - v.x, y - v.y, z - v.z, w - v.w);
}

__inline
Vec4 operator*(const Vec4& vec, float f)
{
    return Vec4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
}

__inline
Vec4 operator*(float f, const Vec4& vec)
{
    return Vec4(vec.x * f, vec.y * f, vec.z * f, vec.w * f);
}