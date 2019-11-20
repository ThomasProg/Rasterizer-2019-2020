#include <cassert>
#include <string.h>
#include <cmath>

#include "matrix4x4.h"


Matrix4x4::Matrix4x4 () 
    : Matrix(4, 4) //matrix = new...
{}

Matrix4x4::Matrix4x4 (const Matrix& copiedMatrix) 
    : Matrix(4, 4)
{
    assert(copiedMatrix.nbLines == 4 && copiedMatrix.nbColumns == 4);

    this->matrix = new float[4 * 4];
    
    memcpy(this->matrix, copiedMatrix.matrix, sizeof(float) * 4 * 4);
}

Matrix4x4 Matrix4x4::CreateScaleMatrix(vector3D scale)
{
    Matrix4x4 matrix;

    memset(matrix.matrix, 0, 4*4);

    matrix.matrix[0]  = scale.x;
    matrix.matrix[5]  = scale.y;
    matrix.matrix[10] = scale.z;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateTranslationMatrix(vector3D translation)
{
    Matrix4x4 matrix;

    for (unsigned int i = 0; i < 4*3; i++)
    {
        matrix.matrix[i] = i % (3 + 1) == 0 ? 1 : 0;
    }

    matrix.matrix[3]  = translation.x;
    matrix.matrix[7]  = translation.y;
    matrix.matrix[11] = translation.z;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateXRotationMatrix(float angleX)
{
    Matrix4x4 matrix;

    matrix.matrix[0]  = 1;
    matrix.matrix[1]  = 0;
    matrix.matrix[2]  = 0;
    matrix.matrix[3]  = 0;

    matrix.matrix[4]  = 0;
    matrix.matrix[5]  = cos(angleX);
    matrix.matrix[6]  = -sin(angleX);
    matrix.matrix[7]  = 0;

    matrix.matrix[8]  = 0;
    matrix.matrix[9]  = sin(angleX);
    matrix.matrix[10]  = cos(angleX);
    matrix.matrix[11]  = 0;

    matrix.matrix[12]  = 0;
    matrix.matrix[13]  = 0;
    matrix.matrix[14]  = 0;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateYRotationMatrix(float angleY)
{
    Matrix4x4 matrix;

    matrix.matrix[0] = cos(angleY);
    matrix.matrix[1] = 0;
    matrix.matrix[2] = sin(angleY);
    matrix.matrix[3] = 0;

    matrix.matrix[4] = 0;
    matrix.matrix[5] = 1;
    matrix.matrix[6] = 0;
    matrix.matrix[7] = 0;

    matrix.matrix[8]  = -sin(angleY);
    matrix.matrix[9]  = 0;
    matrix.matrix[10] = cos(angleY);
    matrix.matrix[11]  = 0;

    matrix.matrix[12] = 0;
    matrix.matrix[13] = 0;
    matrix.matrix[14] = 0;
    matrix.matrix[15] = 1;

    return matrix;
}

Matrix4x4 Matrix4x4::CreateZRotationMatrix(float angleZ)
{
    Matrix4x4 matrix;
    
    matrix.matrix[0] = cos(angleZ);
    matrix.matrix[1] = 0;
    matrix.matrix[2] = sin(angleZ);
    matrix.matrix[3] = 0;

    matrix.matrix[4] = 0;
    matrix.matrix[5] = 1;
    matrix.matrix[6] = 0;
    matrix.matrix[7] = 0;

    matrix.matrix[8]  = -sin(angleZ);
    matrix.matrix[9]  = 0;
    matrix.matrix[10] = cos(angleZ);
    matrix.matrix[11]  = 0;

    matrix.matrix[12] = 0;
    matrix.matrix[13] = 0;
    matrix.matrix[14] = 0;
    matrix.matrix[15] = 1;
}


Matrix4x4 Matrix4x4::CreateFixedAngleEulerRotationMatrix(vector3D angles)
{
    //convert Matrix to Matrix4x4 calling Matrix4x4::Matrix4x4 (const Matrix& copiedMatrix);
    //not opti
    return CreateYRotationMatrix(angles.y) * CreateXRotationMatrix(angles.x) * CreateZRotationMatrix(angles.z);
}


Matrix4x4 Matrix4x4::CreateTRSMatrix(vector3D scale, vector3D translation, vector3D angles)
{
    //convert Matrix to Matrix4x4 calling Matrix4x4::Matrix4x4 (const Matrix& copiedMatrix);
    //not opti
    return CreateScaleMatrix(scale) * CreateFixedAngleEulerRotationMatrix(angles) * CreateTranslationMatrix(translation);
}
