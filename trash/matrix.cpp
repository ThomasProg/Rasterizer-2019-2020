#include <assert.h>
#include <string.h>
#include <iostream>
#include <cmath>
#include "matrix.h"

Matrix::Matrix()
{

}

Matrix::Matrix(unsigned int nbLines, unsigned int nbColumns)
{
    this->nbLines = nbLines;
    this->nbColumns = nbColumns;

    this->matrix = new float[nbLines * nbColumns];
}

Matrix::Matrix(const Matrix& copiedMatrix)
{
    this->nbColumns = copiedMatrix.nbColumns;
    this->nbLines   = copiedMatrix.nbLines;
    this->matrix    = new float[this->nbLines * this->nbColumns];
    
    memcpy(this->matrix, copiedMatrix.matrix, sizeof(float) * this->nbColumns * this->nbLines);
}

Matrix::Matrix(const Matrix* copiedMatrix)
{
    this->nbColumns = copiedMatrix->nbColumns;
    this->nbLines   = copiedMatrix->nbLines;
    this->matrix    = new float[this->nbLines * this->nbColumns];

    float* matrixArray = copiedMatrix->matrix; //for opti 
    
    memcpy(this->matrix, copiedMatrix->matrix, sizeof(float) * this->nbColumns * this->nbLines);
}

Matrix::~Matrix()
{
    if (matrix != nullptr)
        delete matrix;
}

Matrix Matrix::identity(const unsigned int size)
{
    Matrix matrix(size, size);

    unsigned int totalSize = size * size;

    for (unsigned int i = 0; i < totalSize; i++)
    {
        matrix.matrix[i] = i % (size + 1) == 0 ? 1 : 0;
    }

    return matrix;
}

Matrix Matrix::zero(const unsigned int nbLines, const unsigned int nbColumns)
{
    Matrix zeroMatrix;

    zeroMatrix.nbColumns = nbColumns;
    zeroMatrix.nbLines   = nbLines;
    zeroMatrix.matrix    = new float[nbLines * nbColumns];
    
    memset(zeroMatrix.matrix, 0, sizeof(float) * nbColumns * nbLines);

    return zeroMatrix;
}

float Matrix::accessor(const unsigned int line, const unsigned int column)
{
    assert(line < nbLines && column < nbColumns);
    return this->matrix[column + line * this->nbColumns];
}

Matrix Matrix::transpose()
{
    Matrix transposedMatrix(this->nbColumns, this->nbLines);

    for (unsigned int i = 0; i < nbLines; i++)
    {
        for (unsigned int j = 0; j < nbColumns; j++)
        {
            transposedMatrix.matrix[i + j * nbLines] = this->matrix[j + i * nbColumns];    
        }
    }

    return transposedMatrix;
}

Matrix& Matrix::operator=(const Matrix& copiedMatrix)
{
    this->nbColumns = copiedMatrix.nbColumns;
    this->nbLines   = copiedMatrix.nbLines;
    if (this->matrix == nullptr)
        delete this->matrix;

    this->matrix    = new float[this->nbLines * this->nbColumns];
    
    memcpy(this->matrix, copiedMatrix.matrix, sizeof(float) * this->nbColumns * this->nbLines);

    return *this;
}

Matrix Matrix::operator+(Matrix& matrix)
{
    assert(this->nbLines == matrix.nbLines && this->nbColumns == matrix.nbColumns);
    Matrix resultMatrix(this->nbLines, this->nbColumns);

    unsigned int size = nbLines * nbColumns;

    for (unsigned int i = 0; i < size; i++)
        resultMatrix.matrix[i] = this->matrix[i] - matrix.matrix[i];   

    return resultMatrix;
}

Matrix Matrix::operator-(Matrix& matrix)
{
    assert(this->nbLines == matrix.nbLines && this->nbColumns == matrix.nbColumns);
    Matrix resultMatrix(this->nbLines, this->nbColumns);

    unsigned int size = nbLines * nbColumns;

    for (unsigned int i = 0; i < size; i++)
        resultMatrix.matrix[i] = this->matrix[i] - matrix.matrix[i];   

    return resultMatrix;
}

Matrix Matrix::operator*(const Matrix& matrix) const
{
    assert(this->nbColumns == matrix.nbLines); //required for multiplication
    Matrix resultMatrix(matrix.nbLines, this->nbColumns);

    float result;

    for (unsigned int i = 0; i < matrix.nbLines; i++)
    {
        for (unsigned int j = 0; j < this->nbColumns; j++)
        {
            result = 0.f;
            for (unsigned int k = 0; k < this->nbColumns; k++)
            {
                result += this->matrix[i * nbColumns + k] 
                        * matrix.matrix[j + k * nbColumns];    
            }

            resultMatrix.matrix[j + i * nbColumns] = result;
        }
    }

    return resultMatrix;
}

float* Matrix::operator[](unsigned int x) const
{
    return &this->matrix[x * this->nbColumns];
}

void Matrix::addElement(int x, int y, float element)
{
    this->matrix[x + y * this->nbColumns] = element;
}

Matrix Matrix::GetSubMatrix(const Matrix& matrix, unsigned int excludedColumnIndex, unsigned int excludedLineIndex)
{
    Matrix m(matrix.nbLines - 1, matrix.nbColumns - 1);
    unsigned int x = 0, y = 0;
    for (unsigned int iy = 0; iy < matrix.nbLines; iy++)
    {
        if (iy != excludedLineIndex)
        {
            for (unsigned int jx = 0; jx < matrix.nbLines; jx++)
            {
                if (jx != excludedColumnIndex)
                {
                    m[x][y] = matrix[jx][iy];
                    x++;
                }
            }   
            y++;
            x = 0;
        }
    }

    return m;
}

float Matrix::getDeterminant() 
{
    assert(nbLines == nbColumns);
    if (nbLines == 1)
        return matrix[0];
    else if (nbLines == 2)
        return matrix[0] * matrix[3] - matrix[1] * matrix[2];
    else if (nbLines == 3)
        return matrix[0] * (matrix[4] * matrix[8] - matrix[6] * matrix[7])
             - matrix[1] * (matrix[3] * matrix[8] - matrix[5] * matrix[6])
             + matrix[2] * (matrix[3] * matrix[7] - matrix[4] * matrix[6]);
    // if (nbLines == 3)
    //     return (*this)[0][0] * ((*this)[1][1] * (*this)[2][2] - (*this)[1][2] * (*this)[2][1])
    //          - (*this)[0][1] * ((*this)[1][0] * (*this)[2][2] - (*this)[1][2] * (*this)[2][0])
    //          + (*this)[0][2] * ((*this)[1][0] * (*this)[2][1] - (*this)[1][1] * (*this)[2][0]);
    else if (nbLines == 4)
        return matrix[0] 
                * (matrix[5] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) 
                   - matrix[6] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) 
                   + matrix[7] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]))
            -  matrix[1] 
                * (matrix[4] * (matrix[10] * matrix[15] - matrix[11] * matrix[14]) 
                  - matrix[6] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) 
                  + matrix[7] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]))
            +  matrix[2] 
                * (matrix[4] * (matrix[9] * matrix[15] - matrix[11] * matrix[13]) 
                  - matrix[5] * (matrix[8] * matrix[15] - matrix[11] * matrix[12]) 
                  + matrix[7] * (matrix[8] * matrix[13] - matrix[9] * matrix[12]))
            -  matrix[3] 
                * (matrix[4] * (matrix[9] * matrix[14] - matrix[10] * matrix[13]) 
                  - matrix[5] * (matrix[8] * matrix[14] - matrix[10] * matrix[12]) 
                  + matrix[6] * (matrix[8] * matrix[13] - matrix[9] * matrix[12]));
    
    else 
    {
        float det = 0.f;
        for (unsigned int x = 0; x < this->nbLines; x++)
        {
            Matrix subMatrix = GetSubMatrix(*this, 0, x);
            det += (*this)[0][x] * subMatrix.getDeterminant() * std::pow(-1, x);
            //std::cout << "Calcul : det +=" << (*this)[0][x] << " * " << subMatrix.getDeterminant() << " * " << std::pow(-1, x) << std::endl;
            //std::cout << "added : " << (*this)[0][x] * subMatrix.getDeterminant() * std::pow(-1, x) << std::endl;
        }
        return det; 
    }
}
