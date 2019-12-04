#include <cstring>
#include <cmath>

#include "mat4.h"

#include "vec4.h"
#include "vec3.h"
#include "macros.h"

std::array<float, Mat4::nbCase> Mat4::IdentityInArray() 
{
   std::array<float, 16> elements;

   for (unsigned int i = 0; i < nbCase; i++)
        elements[i] = 0;
        
    elements[0]  = 1;
    elements[5]  = 1;
    elements[10] = 1;
    elements[15] = 1;

    return elements;
}

Mat4 Mat4::operator*(const Mat4& matrix) const
{
    Mat4 resultMatrix;

    float result;

    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < this->nbColumns; j++)
        {
            result = 0.f;
            for (unsigned int k = 0; k < this->nbColumns; k++)
            {
                result += this->elements[i * nbColumns + k] 
                        * matrix.elements[j + k * nbColumns];    
            }

            resultMatrix.elements[j + i * nbColumns] = result;
        }
    }
    return resultMatrix;
}

Mat4 Mat4::operator*=(const Mat4& matrix)
{
    Mat4 resultMatrix;

    float result;

    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < this->nbColumns; j++)
        {
            result = 0.f;
            for (unsigned int k = 0; k < this->nbColumns; k++)
            {
                result += this->elements[i * nbColumns + k] 
                        * matrix.elements[j + k * nbColumns];    
            }

            resultMatrix.elements[j + i * nbColumns] = result;
        }
    }
    *this = resultMatrix;
    return *this;
}

Mat4 Mat4::operator=(const Mat4& matrix)
{
    for (unsigned int i = 0; i < nbCase; i++)
    {
        elements[i] = matrix.elements[i];
    }
    return *this;
}

Vec4 Mat4::operator*(const Vec4& vect) const
{
    // Vec4 resultVect(0,0,0,0);
    // for (unsigned int i = 0; i < nbLines; i++)
    // {
    //     for (unsigned int j = 0; j < nbColumns; j++)
    //         resultVect[i] += elements[(i*nbColumns)+j]*vect[j];  
        
    // }
    // return resultVect;

    Vec4 resultVect(0,0,0,0);
    for (unsigned int i = 0; i < nbLines; i++)
    {
        for (unsigned int j = 0; j < nbColumns; j++)
            resultVect[i] += elements[(i*nbColumns)+j]*vect[j];  
        
    }
    return resultVect;
}

Mat4& Mat4::operator*=(float scale) 
{
    for(unsigned i = 0; i < nbCase; i++)
        elements[i] *= scale;

    return *this;
}

Mat4 Mat4::operator*(float scale) const
{
    Mat4 mat;
    
    for(unsigned i = 0; i < nbCase; i++)
        mat.elements[i] *= scale;

    return mat;
}

float* Mat4::operator[](unsigned int x)
{
    return &this->elements[x * this->nbColumns];
}

const float* Mat4::operator[](unsigned int x) const
{
    return &this->elements[x * this->nbColumns];
}

Mat4 Mat4::CreateTransformMatrix(const Vec3& rotation, const Vec3& position, const Vec3& scale)
{
    return CreateTranslationMatrix(position)*
           CreateRotationMatrix(rotation)*
           CreateScaleMatrix(scale);
}

Mat4 Mat4::CreateTranslationMatrix(const Vec3& translation)
{
    Mat4 matrix;
    memset(&matrix.elements,0,nbCase*sizeof(float));

    matrix.elements[0] = 1;
    matrix.elements[3] = translation.x;
    matrix.elements[5] = 1;
    matrix.elements[7] = translation.y;
    matrix.elements[10] =1;
    matrix.elements[11] =translation.z;
    matrix.elements[15] =1;

    return matrix;
    
}
Mat4 Mat4::CreateScaleMatrix(const Vec3& scale)
{
    Mat4 matrix;

    memset(&matrix.elements, 0, nbCase*sizeof(float));

    matrix.elements[0]  = scale.x;
    matrix.elements[5]  = scale.y;
    matrix.elements[10] = scale.z;
    matrix.elements[15] = 1;

    return matrix;
}
Mat4 Mat4::CreateXRotationMatrix(float angle)
{
    Mat4 matrix;
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    matrix.elements[0]  = 1;
    matrix.elements[1]  = 0;
    matrix.elements[2]  = 0;
    matrix.elements[3]  = 0;

    matrix.elements[4]  = 0;
    matrix.elements[5]  = cosAngle;
    matrix.elements[6]  = -sinAngle;
    matrix.elements[7]  = 0;

    matrix.elements[8]  = 0;
    matrix.elements[9]  = sinAngle;
    matrix.elements[10]  = cosAngle;
    matrix.elements[11]  = 0;

    matrix.elements[12]  = 0;
    matrix.elements[13]  = 0;
    matrix.elements[14]  = 0;
    matrix.elements[15] = 1;

    return matrix;
}
Mat4 Mat4::CreateYRotationMatrix(float angle)
{
    Mat4 matrix;
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);

    matrix.elements[0] = cosAngle;
    matrix.elements[1] = 0;
    matrix.elements[2] = sinAngle;
    matrix.elements[3] = 0;

    matrix.elements[4] = 0;
    matrix.elements[5] = 1;
    matrix.elements[6] = 0;
    matrix.elements[7] = 0;

    matrix.elements[8]  = -sinAngle;
    matrix.elements[9]  = 0;
    matrix.elements[10] = cosAngle;
    matrix.elements[11]  = 0;

    matrix.elements[12] = 0;
    matrix.elements[13] = 0;
    matrix.elements[14] = 0;
    matrix.elements[15] = 1;

    return matrix;
}
Mat4 Mat4::CreateZRotationMatrix(float angle)
{
    Mat4 matrix;
    float cosAngle = cos(angle);
    float sinAngle = sin(angle);
    
    matrix.elements[0] = cosAngle;
    matrix.elements[1] = -sinAngle;
    matrix.elements[2] = 0;
    matrix.elements[3] = 0;

    matrix.elements[4] = sinAngle;
    matrix.elements[5] = cosAngle;
    matrix.elements[6] = 0;
    matrix.elements[7] = 0;

    matrix.elements[8]  = 0;
    matrix.elements[9]  = 0;
    matrix.elements[10] = 1;
    matrix.elements[11] = 0;

    matrix.elements[12] = 0;
    matrix.elements[13] = 0;
    matrix.elements[14] = 0;
    matrix.elements[15] = 1;

    return matrix;
}

Mat4 Mat4::CreateRotationMatrix(const Vec3& rotation)
{
    return CreateYRotationMatrix(rotation.y)*
           CreateXRotationMatrix(rotation.x)*
           CreateZRotationMatrix(rotation.z);
}

std::ostream& operator<<(std::ostream& stream, const Mat4& matrix)
{
    stream << "\n";
    
    for (unsigned int i = 0; i < 4; i++)
    {
        for (unsigned int j = 0; j < 4; j++)
        {
            std::cout << matrix[i][j] << "  ";
        }
        std::cout << std::endl;
    }

    return stream;
}

Mat4 Mat4::CreatePerspectiveProjectionMatrix(int width, int height,float near,float far,float fov)
{
    float ymax, xmax;
    ymax = tanf(fov * PI / 360.f);
    xmax = ymax * width / height;

    Mat4 m;

    float temp4 = far - near;
    
    m.elements[0] = 1/xmax;
    m.elements[1] = 0.0;
    m.elements[2] = 0.0;
    m.elements[3] = 0.0;
    m.elements[4] = 0.0;
    m.elements[5] = 1/ymax;
    m.elements[6] = 0.0;
    m.elements[7] = 0.0;
    m.elements[8] = 0.f;
    m.elements[9] = 0.f;
    m.elements[10] = -(far + near) / temp4;
    m.elements[11] = -1;
    m.elements[12] = 0.0;
    m.elements[13] = 0.0;
    m.elements[14] = -2 * (far * near) / temp4;
    m.elements[15] = 0.0;

    return m;
}

Mat4 Mat4::CreateOrthogonalProjectionMatrix()
{
    Mat4 ortho;

    // ortho.elements[0] = windowWidth/2;
    // ortho.elements[1] = 0;
    // ortho.elements[2] = 0;
    // ortho.elements[3] = windowWidth/2;

    // ortho.elements[4] = 0;
    // ortho.elements[5] = windowHeight/2;
    // ortho.elements[6] = 0;
    // ortho.elements[7] = windowHeight/2;
    
    // ortho.elements[8] = 0;
    // ortho.elements[9] = 0;
    // ortho.elements[10] = 1;
    // ortho.elements[11] = 0;

    // ortho.elements[12] = 0;
    // ortho.elements[13] = 0;
    // ortho.elements[14] = 0;
    // ortho.elements[15] = 1;

    ortho.elements[0] = 1;
    ortho.elements[1] = 0;
    ortho.elements[2] = 0;
    ortho.elements[3] = 0;

    ortho.elements[4] = 0;
    ortho.elements[5] = 1;
    ortho.elements[6] = 0;
    ortho.elements[7] = 0;
    
    ortho.elements[8] = 0;
    ortho.elements[9] = 0;
    ortho.elements[10] = 1;
    ortho.elements[11] = 0;

    ortho.elements[12] = 0;
    ortho.elements[13] = 0;
    ortho.elements[14] = 0;
    ortho.elements[15] = 1;
    
    return ortho;
}

Mat4 Mat4::CreateScreenConversionMatrix()
{
    Mat4 screen;

    screen.elements[0] = 2.f/5.f * windowWidth/2;
    screen.elements[1] = 0;
    screen.elements[2] = 0;
    screen.elements[3] = windowWidth/2;

    screen.elements[4] = 0;
    screen.elements[5] = 2.f/5.f * windowHeight/2;
    screen.elements[6] = 0;
    screen.elements[7] = windowHeight/2;
    
    screen.elements[8] = 0;
    screen.elements[9] = 0;
    screen.elements[10] = 1;
    screen.elements[11] = -1;

    screen.elements[12] = 0;
    screen.elements[13] = 0;
    screen.elements[14] = 0;
    screen.elements[15] = 1;
    
    return screen;
}

float Mat4::det_2(unsigned x,unsigned y,unsigned z,unsigned w) const
{
    return (elements[x]*elements[y] - elements[z] *elements[w]);
}

Mat4 Mat4::CoMatrix() const
{   
    Mat4 coM;
    float temp1 = det_2(10,15,14,11);
    float temp2 = det_2(14,7,6,15);
    float temp3 = det_2(6,11,10,7);

    coM.elements[0] = (elements[5]*temp1 + elements[9]*temp2 + elements[13]*temp3);
    coM.elements[1] = -(elements[4]*temp1 + elements[8]*temp2 + elements[12]*temp3); //check  
    coM.elements[2] = (elements[4]*det_2(9,15,13,11) + elements[8]*det_2(13,7,5,15) + elements[12]*det_2(5,11,9,7));  
    coM.elements[3] = -(elements[4]*det_2(9,14,13,10) + elements[8]*det_2(13,6,5,14) + elements[12]*det_2(5,10,9,6));

    temp2 = det_2(14,3,2,15);
    temp3 = det_2(2,11,10,3);

    coM.elements[4] = -(elements[1]*temp1 + elements[9]*temp2 + elements[13]*temp3);
    coM.elements[5] = (elements[0]*temp1 + elements[8]*temp2 + elements[12]*temp3); //check
    
    coM.elements[6] = -(elements[0]*det_2(9,15,13,11) + elements[8]*det_2(13,3,1,15) + elements[12]*det_2(1,11,9,3));
    coM.elements[7] = (elements[0]*det_2(9,14,13,10) + elements[8]*det_2(13,2,1,14) + elements[12]*det_2(1,10,9,2));
    
    coM.elements[8] = (elements[1]*det_2(6,15,14,7) + elements[5]*det_2(14,3,2,15) + elements[13]*det_2(2,7,6,3));
    coM.elements[9] = -(elements[0]*det_2(6,15,14,7) + elements[4]*det_2(14,3,2,15) + elements[12]*det_2(2,7,6,3));
    coM.elements[10] = (elements[0]*det_2(5,15,13,7) + elements[4]*det_2(13,3,1,15) + elements[12]*det_2(1,7,5,3));
    coM.elements[11] = -(elements[0]*det_2(5,14,13,6) + elements[4]*det_2(13,2,1,14) + elements[12]*det_2(1,6,5,2));

    coM.elements[12] = -(elements[1]*det_2(6,11,10,7) + elements[5]*det_2(10,3,2,11) + elements[9]*det_2(2,7,6,3));
    coM.elements[13] = (elements[0]*det_2(6,11,10,7) + elements[4]*det_2(10,3,2,11) + elements[8]*det_2(2,7,6,3));
    coM.elements[14] = -(elements[0]*det_2(5,11,9,7) + elements[4]*det_2(9,3,1,11) + elements[8]*det_2(1,7,5,3));
    coM.elements[15] = (elements[0]*det_2(5,10,9,6) + elements[4]*det_2(9,2,1,10) + elements[8]*det_2(1,6,5,2));


    return coM;
}

Mat4 Mat4::GetInverse() const
{
    Mat4 inverse;
    Mat4 AdjM = this->CoMatrix().Transpose();

    inverse.elements[0] = AdjM.elements[0]/determinant();
    inverse.elements[1] = AdjM.elements[1]/determinant();
    inverse.elements[2] = AdjM.elements[2]/determinant();
    inverse.elements[3] = AdjM.elements[3]/determinant();

    inverse.elements[4] = AdjM.elements[4]/determinant();
    inverse.elements[5] = AdjM.elements[5]/determinant();
    inverse.elements[6] = AdjM.elements[6]/determinant();
    inverse.elements[7] = AdjM.elements[7]/determinant();
    
    inverse.elements[8] = AdjM.elements[8]/determinant();
    inverse.elements[9] = AdjM.elements[9]/determinant();
    inverse.elements[10] = AdjM.elements[10]/determinant();
    inverse.elements[11] = AdjM.elements[11]/determinant();

    inverse.elements[12] = AdjM.elements[12]/determinant();
    inverse.elements[13] = AdjM.elements[13]/determinant();
    inverse.elements[14] = AdjM.elements[14]/determinant();
    inverse.elements[15] = AdjM.elements[15]/determinant();

    return inverse;
}
float Mat4::determinant() const
{

    return (elements[0] * (elements[5] * det_2(10,15,14,11) + elements[9] * det_2(14,7,6,15) + elements[13] * det_2(6,11,10,7)) -
            elements[1] * (elements[4] * det_2(10,15,14,11) + elements[8] * det_2(14,7,6,15) + elements[12] * det_2(6,11,10,7)) +
            elements[2] * (elements[4] * det_2(9,15,13,11 ) + elements[8] * det_2(13,7,5,15) + elements[12] * det_2(5,11,9,7) ) -
            elements[3] * (elements[4] * det_2(9,14,13,10 ) + elements[8] * det_2(13,6,5,14) + elements[12] * det_2(5,10,9,6)));
}

Mat4 Mat4::Transpose() const
{
    Mat4 Transpose;
    Transpose.elements[0] = elements[0];
    Transpose.elements[1] = elements[4];
    Transpose.elements[2] = elements[8];
    Transpose.elements[3] = elements[12];

    Transpose.elements[4] = elements[1];
    Transpose.elements[5] = elements[5];
    Transpose.elements[6] = elements[9];
    Transpose.elements[7] = elements[13];
    
    Transpose.elements[8] = elements[2];
    Transpose.elements[9] = elements[6];
    Transpose.elements[10] = elements[10];
    Transpose.elements[11] = elements[14];
    
    Transpose.elements[12] = elements[3];
    Transpose.elements[13] = elements[7];
    Transpose.elements[14] = elements[11];
    Transpose.elements[15] = elements[15];

    return Transpose;
}