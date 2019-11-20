#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <array>

class Matrix
{
private:
public:
    float* matrix = nullptr;
    unsigned int nbLines, nbColumns;    
public:
    Matrix();
    Matrix(unsigned int nbLines, unsigned int nbColumns);
    Matrix(const Matrix&);
    Matrix(const Matrix*);
    ~Matrix();

    static Matrix identity(const unsigned int size);
    static Matrix zero(const unsigned int nbLines, const unsigned int nbColumns);

    float accessor(const unsigned int line, const unsigned int column);

    Matrix transpose();
    Matrix&operator=(const Matrix& copiedMatrix);
    Matrix operator+(Matrix& matrix);
    Matrix operator-(Matrix& matrix);
    Matrix operator*(const Matrix& matrix) const;
    float* operator[](unsigned int id) const;

    Matrix operator/=(float f);

    bool operator==(const Matrix& matrix) const;

    void addElement(int x, int y, float element);

    static Matrix GetSubMatrix(const Matrix& matrix, unsigned int excludedColumnIndex, unsigned int excludedLineIndex);
    float getDeterminant();

    bool isOrthogonal();
    float getMinorant(unsigned int excludedColumnIndex, unsigned int excludedLineIndex) const;
    float getCofactor(unsigned int column, unsigned int line) const;
    Matrix getComatrix();
    Matrix getAdjoint();
    Matrix getInverse();
    Matrix resolveEquation();
};



#endif