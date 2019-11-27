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
    Mat4 perspective;
    float ratio = width/height;
    perspective.elements[0] =  1/(ratio*tan(fov/2));
    perspective.elements[1] =  0;
    perspective.elements[2] =  0;
    perspective.elements[3] =  0;

    perspective.elements[4] =  0;
    perspective.elements[5] =  1/tan(fov/2);
    perspective.elements[6] =  0;
    perspective.elements[7] =  0;
    
    perspective.elements[8] =  0;
    perspective.elements[9] =  0;
    perspective.elements[10] = (near+far)/(near-far);
    perspective.elements[11] = 2*(near+far)/(near-far);

    perspective.elements[12] = 0;
    perspective.elements[13] = 0;
    perspective.elements[14] = -1;
    perspective.elements[15] = 0;

    // Mat4 perspective;
    // float ratio = width/height;
    // perspective.elements[0] =  0.5;
    // perspective.elements[1] =  0;
    // perspective.elements[2] =  0;
    // perspective.elements[3] =  0;

    // perspective.elements[4] =  0;
    // perspective.elements[5] =  0.5;
    // perspective.elements[6] =  0;
    // perspective.elements[7] =  0;
    
    // perspective.elements[8] =  0;
    // perspective.elements[9] =  0;
    // perspective.elements[10] = 1;
    // perspective.elements[11] = 0;

    // perspective.elements[12] = 0;
    // perspective.elements[13] = 0.0;
    // perspective.elements[14] = 1.0;
    // perspective.elements[15] = 1;

    return perspective;
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