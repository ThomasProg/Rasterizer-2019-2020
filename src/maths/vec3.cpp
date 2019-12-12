#include <cmath>
#include <cassert>

#include "vec3.h"
#include "vec4.h"

Vec3::Vec3()
{
    
}

Vec3::Vec3(float x, float y, float z)
    : x(x), 
      y(y),
      z(z)
{

}

Vec3::Vec3(const Vec3& rhs)
    : x(rhs.x), 
      y(rhs.y),
      z(rhs.z)
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

float& Vec3::operator[](unsigned int index)
{
    assert(index < 3);

    switch (index)
    {
    case 0:
        return x;
        break;

    case 1:
        return y;
        break;

    default: //default is last
        return z; 
        break;
    }
}

Vec3 Vec3::getNormalized() const
{
    return *this * (1 / this->GetMagnitude());
}

float Vec3::getLengthSquared() const
{
    return pow(x, 2) + pow(y, 2) + pow(z, 2);
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

Vec3 operator/(const Vec3& vec, float f)
{
    return Vec3(vec.x / f, vec.y / f, vec.z / f);
}

Vec3& Vec3::operator+=(const Vec3& vec)
{
    x += vec.x;
    y += vec.y;
    z += vec.z;
    return *this;
}

Vec3& Vec3::operator/=(float f)
{
    x /= f;
    y /= f;
    z /= f;
    return *this;
}

std::ostream& operator<<(std::ostream& stream, const Vec3& vector)
{
    stream << "x : " << vector.x << " | y : " << vector.y << " | z : " << vector.z << std::endl;
    return stream;
}

float dotProduct(Vec3 vect1, Vec3 vect2)
{
    return (vect1.x*vect2.x)+(vect1.y*vect2.y)+(vect1.z*vect2.z);
}
Vec3 Vec3::operator-(const Vec3& vect) const
{
    return Vec3(x - vect.x, y - vect.y, z - vect.z);
}

Vec3 crossProduct(Vec3 v1, Vec3 v2)
{
    return Vec3
    (
        v1.y * v2.z - v1.z * v2.y,
        v1.z * v2.x - v1.x * v2.z,
        v1.x * v2.y - v1.y * v2.x
    );
}

