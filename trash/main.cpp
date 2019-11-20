
#include <iostream>
#include <chrono>
#include "matrix.h"

void matrix_print(Matrix& result)
{
    for (unsigned int i = 0; i < result.nbLines; i++)
    {
        for (unsigned int j = 0; j < result.nbColumns; j++)
        {
            std::cout << result.matrix[j + i * result.nbColumns];
        }
        std::cout << std::endl;
    }
}

void test()
{
    Matrix z(3, 3);

    for (unsigned int i = 0; i < z.nbLines; i++)
    {
        for (unsigned int j = 0; j < z.nbColumns; j++)
        {
            z.matrix[j + i * z.nbColumns] = j + i * z.nbColumns + 2.f;
        }
    }

    Matrix x;
    x = Matrix::identity(3);
    Matrix result;
    result = x*z;
    
    auto start = std::chrono::system_clock::now();
    for (unsigned int i = 0; i < 1000; i++)
    {
        result = Matrix::zero(3, 3);
        delete result.matrix;
    }
    result = Matrix::identity(5);
    auto end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_seconds = end-start;
    std::time_t end_time = std::chrono::system_clock::to_time_t(end);
 
    std::cout << "finished computation at " << std::ctime(&end_time)
              << "elapsed time: " << elapsed_seconds.count() << "s\n";

    //matrix_print(result);

    Matrix n = Matrix::zero(5,3);
    n[2][0] = 5;
    matrix_print(n);
    Matrix transposey = n.transpose();
    matrix_print(transposey);

    std::cout << z[1][2] << std::endl;
}

void detTest()
{
    // {
    //     Matrix m1(2, 2);
    //     m1[0][0] = 1;
    //     m1[0][1] = 2;
    //     m1[1][0] = 3;
    //     m1[1][1] = 4;

    //     matrix_print(m1);

    //     std::cout << "det : " << m1.getDeterminant() << std::endl;
    // }

    {
        Matrix m1(4, 4);
        m1[0][0] = 1;
        m1[0][1] = -4;
        m1[0][2] = 2;
        m1[0][3] = -2;
        m1[1][0] = 4;
        m1[1][1] = 7;
        m1[1][2] = -3;
        m1[1][3] = 5;
        m1[2][0] = 3;
        m1[2][1] = 0;
        m1[2][2] = 8;
        m1[2][3] = 0;
        m1[3][0] = -5;
        m1[3][1] = -1;
        m1[3][2] = 6;
        m1[3][3] = 9;

        matrix_print(m1);
        std::cout << "det : " << m1.getDeterminant() << std::endl;

        std::cout << " M2 " << std::endl;
        Matrix m2 = Matrix::GetSubMatrix(m1, 0, 0);
        matrix_print(m2);
    }
}



// #include <functional>

// void myLoop(unsigned int n, unsigned int max, const std::function<void(unsigned int i)> func)
// {
//     func(n);
//     if (n < max)
//         myLoop(n + 1, max, func);
// }


// int main()
// {
//     myLoop(0, 10, [](unsigned int i)
//     {
//         std::cout << i << std::endl;
//     });
// }

int main()
{
    detTest();
}
