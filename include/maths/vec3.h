#ifndef _VEC3_H_
#define _VEC3_H_

#include <iostream>

struct Vec4;

struct Vec3
{
public:
    union 
    {
        struct 
        {
            float x, y, z;
        };

        struct 
        {
            float arr[3];
        };
    };

    __inline
    Vec3();
    __inline
    Vec3(float f);
    __inline
    Vec3(float x, float y, float z);
    __inline
    Vec3(const Vec4&);
    __inline
    Vec3(const Vec3&);

    __inline
    float GetMagnitude() const;
    __inline
    void  Normalize();
    __inline
    float& operator[](unsigned int index);
    __inline
    Vec3 operator-(const Vec3&) const;
    __inline
    Vec3& operator+=(const Vec3& vec);
    __inline
    Vec3& operator/=(float f);

    __inline
    Vec3 getNormalized() const;

    __inline
    float getLengthSquared() const;
};

__inline
Vec3 operator+(const Vec3&, const Vec3&);

__inline
Vec3 operator*(const Vec3&, float);
__inline
Vec3 operator*(float, const Vec3&);
__inline
Vec3 operator/(const Vec3& vec, float f);

std::ostream& operator<<(std::ostream& stream, const Vec3& vector);

__inline
float dotProduct(Vec3 vect1, Vec3 vect2);
__inline
Vec3 crossProduct(Vec3 v1, Vec3 v2);

#include "vec3.inl"

#endif