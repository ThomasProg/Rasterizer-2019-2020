#ifndef _MAT_4_H_
#define _MAT_4_H_

class Vec3;
class Vec4;

#include <iostream>
#include <array>

struct Mat4
{
private:
    static constexpr unsigned int nbLines = 4, 
                                  nbColumns = 4, 
                                  nbCase = nbColumns*nbLines;

    static std::array<float, nbCase> IdentityInArray();
public:
    std::array<float, nbCase> elements = IdentityInArray();

    
    Mat4 operator*(const Mat4&) const;
    Mat4& operator*=(float scale);
    Mat4 operator*(float scale) const;
    Mat4 operator*=(const Mat4&);
    Mat4 operator=(const Mat4&);
    Vec4 operator*(const Vec4&) const;
    float* operator[](unsigned int id);
    const float* operator[](unsigned int id) const;

    // The following functions has been designed for only Mat4 matrices.
    // It gives a performance gains.
    float determinant() const;
    float det_2(unsigned x,unsigned y,unsigned z,unsigned w) const ;
    // returns comatrix of a Mat4 matrix 
    Mat4 CoMatrix() const;  
    Mat4 Transpose() const;

    static Mat4 CreateTransformMatrix(const Vec3& rotation, const Vec3& position, const Vec3& scale);

    static Mat4 CreateTranslationMatrix(const Vec3& translation);
    static Mat4 CreateScaleMatrix(const Vec3& scale);
    static Mat4 CreateXRotationMatrix(float angle);
    static Mat4 CreateYRotationMatrix(float angle);
    static Mat4 CreateZRotationMatrix(float angle);
    static Mat4 CreateRotationMatrix(const Vec3& rotation);

    static Mat4 CreatePerspectiveProjectionMatrix(int width, int height,float near = 0,float far = 2,float fov = 1);
    static Mat4 CreateOrthogonalProjectionMatrix();
    static Mat4 CreateScreenConversionMatrix();

    Mat4 GetInverse() const;
};

std::ostream& operator<<(std::ostream& stream, const Mat4& vector);

#endif