#ifndef _VEC4_H_
#define _VEC4_H_

#include <iostream>

struct Vec3;
class  Mat4;

struct Vec4
{
public:
    union 
    {
        struct 
        {
            float x, y, z, w;
        };

        struct 
        {
            float arr[4];
        };
        
    };

    __inline
    Vec4() {}
    __inline
    Vec4(const Vec4& vec);
    __inline
    Vec4(float x, float y, float z, float w);
    __inline
    Vec4(const Vec3& vec3, float w = 1.f);

    Vec4 operator*(const Mat4&) const = delete; //column convention
    __inline
    Vec4 operator-(const Vec4&) const;
    __inline
    float& operator[](unsigned int index);
    __inline
    float operator[](unsigned int index) const;

    __inline
    bool  isInsideWSizedCube();
    __inline
    float GetMagnitude() const;
    __inline
    void  Normalize();
    __inline
    void  Homogenize();
    __inline
    Vec3  getHomogenizedVec() const;
};

__inline
Vec4 operator+(const Vec4&, const Vec4&);
__inline
Vec4 operator*(const Vec4&, float);

std::ostream& operator<<(std::ostream& stream, const Vec4& vector);

#include "vec4.inl"

#endif