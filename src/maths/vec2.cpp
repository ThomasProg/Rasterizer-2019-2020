#include <cmath>

#include "vec2.h"

#include "vec3.h"

Vec2::Vec2()
{
    
}

Vec2::Vec2(float x, float y)
    : x(x), y(y)
{

}

Vec2::Vec2(const Vec2& vec)
    : x(vec.x), y(vec.y)
{

}

Vec2::Vec2(const Vec3& vec)
    : x(vec.x), y(vec.y)
{

}

float Vec2::GetMagnitude() const
{
    return sqrt(pow(x, 2) + pow(y, 2));
}

void Vec2::toUnit()
{
    float magnitude = GetMagnitude();
    x /= magnitude;
    y /= magnitude;
}