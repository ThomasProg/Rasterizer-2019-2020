#ifndef _MATRIX_4x4_H_
#define _MATRIX_4x4_H_

#include "matrix.h"

struct vector3D
{
    float x, y, z;
};

// struct transform
// {
//     vector3D scale;
//     vector3D rotation;
//     vector3D location;
// };

class Matrix4x4 : public Matrix
{
private:

public:
    Matrix4x4 ();

    Matrix4x4 (const Matrix& copiedMatrix);

    static Matrix4x4 CreateScaleMatrix(vector3D scale);
    static Matrix4x4 CreateTranslationMatrix(vector3D translation);

    static Matrix4x4 CreateXRotationMatrix(float angleX);
    static Matrix4x4 CreateYRotationMatrix(float angleY);
    static Matrix4x4 CreateZRotationMatrix(float angleZ);

    static Matrix4x4 CreateFixedAngleEulerRotationMatrix(vector3D angles);

    static Matrix4x4 CreateTRSMatrix(vector3D scale, vector3D translation, vector3D angles);
};

#endif