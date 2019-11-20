// #include <iostream>
// #include "mat4.h"
// #include "vec4.h"
// #include "vec3.h"

// #include "macros.h"

// //  void printMatrix(const Mat4& mat)
// //  {
// //      for (unsigned int i = 0; i < 4; i++)
// //      {
// //          for (unsigned int j = 0; j < 4; j++)
// //          {
// //              std::cout << mat[i][j] << "  ";
// //          }
// //          std::cout << std::endl;
// //      }
// //  }

// //  void printVec4(const Vec4 vec) 
// //  {
// //      std::cout << vec.x << " " << vec.y << " " << vec.z << " " << vec.w << std::endl;
// //  }

// void testMulti()
// {
//     {
//         Mat4 m1;
//         m1 = m1 * 2.f;
//         m1[0][0] = 5;
//         Mat4 m2;
//         Mat4 m3 = m1 * m2;
//         std::cout << m3;

//         Vec4 v1(2,5,8,1);
//         Vec4 v2 = m1 * v1; 
//         std::cout << v2;
//     }

//     std::cout << "-----hard----" << std::endl;

//     {
//         Mat4 m1;
//         m1[0][0] = 2;
//         m1[0][1] = 1;
//         m1[0][2] = 5;
//         m1[0][3] = 6;

//         m1[1][0] = 4;
//         m1[1][1] = 3;
//         m1[1][2] = 7;
//         m1[1][3] = 10;

//         m1[2][0] = 5;
//         m1[2][1] = 3;
//         m1[2][2] = 13;
//         m1[2][3] = 2;

//         m1[3][0] = 0;
//         m1[3][1] = 1;
//         m1[3][2] = 8;
//         m1[3][3] = 7;

//         Mat4 m2;
//         m2[0][0] = 2;
//         m2[0][1] = 1;
//         m2[0][2] = 5;
//         m2[0][3] = 6;

//         m2[1][0] = 4;
//         m2[1][1] = 3;
//         m2[1][2] = 7;
//         m2[1][3] = 10;

//         m2[2][0] = 5;
//         m2[2][1] = 3;
//         m2[2][2] = 13;
//         m2[2][3] = 2;

//         m2[3][0] = 0;
//         m2[3][1] = 1;
//         m2[3][2] = 8;
//         m2[3][3] = 7;

//         std::cout << m1;
//         std::cout << m2;

//         Mat4 m3 = m1 * m2;
//         std::cout << m3;

//         Vec4 v1(2,5,8,1);
//         Vec4 v2 = m1 * v1; 
//         std::cout << v2;
//     }

//     std::cout << "-----------" << std::endl;
// }

// void testTranslation()
// {
//     Vec3 vec(5,7,9);
//     Mat4 m1 = Mat4::CreateTranslationMatrix(vec);
//     std::cout << (m1);

//     Vec3 loc(-2,4,7);
//     {
//         Vec3 newLoc = loc + vec;
//         std::cout << " new loc (by vec): ";
//         std::cout << (newLoc);
//     }

//     {
//         Vec4 loc4(loc);
//         std::cout << (loc4);
//         Vec4 newLoc = m1 * loc4;
//         std::cout << " new loc (by Matrix): ";
//         std::cout << (newLoc);
//     }

//     std::cout << "-----------" << std::endl;
// }

// void testScale()
// {
//     Vec3 scale(5,2,1);
//     Mat4 mat1 = Mat4::CreateScaleMatrix(scale);
//     std::cout << (mat1);

//     Vec3 currentScale(-2,3,4);
//     {
//         Vec4 currentScale4(currentScale);
//         Vec4 newLoc = mat1 * currentScale4;
//         std::cout << " new loc (by Matrix): ";
//         std::cout << (newLoc);
//     }

//     std::cout << "-----------" << std::endl;
// }

// void testRotation()
// {
//     Vec3 rot(PI/2,PI,0);
//     Mat4 mat1 = Mat4::CreateRotationMatrix(rot);
//     std::cout << (mat1);
// }

// void testRotX()
// {
//     // {
//     //     Mat4 mat1 = Mat4::CreateXRotationMatrix(PI);
//     //     std::cout << "PI : \n "<< (mat1);
//     // }
//     // {
//     //     Mat4 mat1 = Mat4::CreateXRotationMatrix(PI / 2.f);
//     //     std::cout << "PI/2 : \n "<< (mat1);
//     // }
//     // {
//     //     Mat4 mat1 = Mat4::CreateXRotationMatrix(PI / 4.f);
//     //     std::cout << "PI/4 : \n "<< (mat1);
//     // }

//     Vec4 loc(1,0,0,1);
//     Mat4 mat = Mat4::CreateYRotationMatrix(PI/2);
//     Vec4 total = mat * loc;
//     std::cout << total << std::endl;
// }

// void testRotY()
// {
//     {
//         Mat4 mat1 = Mat4::CreateYRotationMatrix(PI);
//         std::cout << "PI : \n "<< (mat1);
//     }

//     {
//         Mat4 mat1 = Mat4::CreateYRotationMatrix(PI / 2.f);
//         std::cout << "PI/2 : \n "<< (mat1);
//     }

//     {
//         Mat4 mat1 = Mat4::CreateYRotationMatrix(PI / 4.f);
//         std::cout << "PI/4 : \n " << (mat1);
//     }
// }

// int main()
// {
//     testMulti();

//     //testTranslation();

//     //testScale();

//     testRotX();

    
//     testRotation();
// }