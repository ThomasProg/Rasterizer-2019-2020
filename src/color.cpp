#include <cassert>
#include "color.h"

// Color::Color()
//     : r(0),
//       g(0),
//       b(0),
//       a(1)
// {

// }

// Color::Color(unsigned char r, unsigned char g, unsigned char b)
//   : r(r), g(g), b(b), a(255) //255 = char max
// {

// }

// Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
//   : r(r), g(g), b(b), a(a)
// {

// }

// Color::Color(const Color& setColor)
//     : r(setColor.r),
//       g(setColor.g),
//       b(setColor.b),
//       a(setColor.a)
// {

// }

// Color& Color::operator=(Color setColor) noexcept
// {
//     r = setColor.r;
//     g = setColor.g;
//     b = setColor.b;
//     a = setColor.a;

//     return *this;
// }

// Color& Color::operator*=(float scalar) noexcept
// {
//   if (scalar <= 0)
//   {
//     r = 0;
//     g = 0;
//     b = 0;
//     return *this;
//   }

//   //since -1 < intensity < 1, we need to lerp the intensity for the color
//   if (r > 255.f / scalar)
//       r = 255;
//   else 
//       r *= scalar;
      
//   if (g > 255.f / scalar)
//       g = 255;
//   else
//       g *= scalar;

//   if (b > 255.f / scalar)
//       b = 255;       
//   else 
//       b *= scalar;  

//   return *this;
// }

// Color Color::operator/(unsigned char f) noexcept
// {
//   return Color(r / f, g / f, b / f, a / f);
// }

// Color& Color::operator+=(const Color& rhs) noexcept
// {
//     r += rhs.r;
//     g += rhs.g;
//     b += rhs.b;
//     return *this;
// }

// // unsigned char& Color::operator[](unsigned int index) noexcept
// // {
// //   switch (index)
// //   {
// //   case 0:
// //     return r;
// //     break;

// //   case 1:
// //     return g;
// //     break;
  
// //   case 2:
// //     return b;
// //     break;

// //   case 3:
// //     return a;
// //     break;

// //   default:
// //     return r;
// //     break;
// //   }
// // }

// Color Color::operator*(float scalar) noexcept
// {
//   Color c = *this;
//   if (scalar <= 0)
//   {
//     c.r = 0;
//     c.g = 0;
//     c.b = 0;

//     return c;
//   }

//   #define multiplyAndClampChar(result, c, scalar) \
//     if (c > 255 / scalar)\
//       result = 255;        \
//     else                \
//       result = c * scalar;

//   multiplyAndClampChar(c.r, this->r, scalar) 
//   multiplyAndClampChar(c.g, this->g, scalar) 
//   multiplyAndClampChar(c.b, this->b, scalar) 

//   //TODO : TODELETE

//   // //since -1 < intensity < 1, we need to lerp the intensity for the color
//   // if (r > 255.f / scalar)
//   //     c.r = 255;
//   // else 
//   //     c.r *= scalar;
      
//   // if (g > 255.f / scalar)
//   //     c.g = 255;
//   // else
//   //     c.g *= scalar;

//   // if (b > 255.f / scalar)
//   //     c.b = 255;       
//   // else 
//   //     c.b *= scalar;  

//   #undef multiplyAndClampChar

//   return c;
// }

// Color Color::operator+(const Color& rhs) noexcept
// {
//   Color c;
//   #define addAndClampChar(result, f1, f2) \
//     if (f1 > 255 - f2)\
//       result = 255;        \
//     else                \
//       result = f1 + f2;

//   addAndClampChar(c.r, this->r, rhs.r);
//   addAndClampChar(c.g, this->g, rhs.g);
//   addAndClampChar(c.b, this->b, rhs.b);

//   #undef addAndClampChar

//   return c;
// }

Color::Color()
    : r(0),
      g(0),
      b(0),
      a(1)
{

}

Color::Color(float r, float g, float b)
  : r(r), g(g), b(b), a(1.f)
{

}

Color::Color(float r, float g, float b, float a)
  : r(r), g(g), b(b), a(a)
{

}

Color::Color(const Color& setColor)
  : r(setColor.r), g(setColor.g), b(setColor.b), a(setColor.a)
{

}

//sizeof(Color) == 4, 
//so we use less memory copying by copy instead of reference (using 4 more bytes)
Color& Color::operator=(Color setColor) noexcept
{
  r = setColor.r;
  g = setColor.g;
  b = setColor.b;
  a = setColor.a;
  return *this;
}

Color& Color::operator*=(float scalar) noexcept
{
  if (scalar <= 0)
  {
    r = 0;
    g = 0;
    b = 0;
    return *this;
  }
  else
  {
    r *= scalar;
    g *= scalar;
    b *= scalar;
    a *= scalar;
  }
  return *this;
}

Color  Color::operator*(float scalar) noexcept
{
  if (scalar <= 0)
  {
    return Color(0, 0, 0, 0);
  }
  else
    return Color(r*scalar, g*scalar, b*scalar, a*scalar);
}

Color  Color::operator+(const Color& rhs) noexcept
{
  return Color(r+rhs.r, g+rhs.g, b+rhs.b, a+rhs.a);
}

Color  Color::operator/(unsigned char f) noexcept
{
  assert(f != 0);
  return Color(r/f, g/f, b/f, a/f);
}

Color& Color::operator+=(const Color& rhs) noexcept
{
  r += rhs.r;
  g += rhs.g;
  b += rhs.b;
  a += rhs.a;

  return *this;
}

//float& operator[](unsigned int index) noexcept;

float Color::getTransparence() const noexcept
{
  return this->a;
}
// #include <iostream>
// void Color::clamp()
// {
//   r = std::min(1.f, r);
//   g = std::min(1.f, g);
//   b = std::min(1.f, b);
//   a = std::min(1.f, a);

//   r = std::max(0.f, r);
//   g = std::max(0.f, g);
//   b = std::max(0.f, b);
//   a = std::max(0.f, a);
// }

std::ostream& operator<<(std::ostream& stream, const Color& c)
{
  stream << c.r << ' ' << c.g << ' ' << c.b << ' ' << c.a << '\n';
  return stream;
}

Color getAverageColor(Color lhs, Color rhs, float lhsRatio) noexcept
{
  Color newColor = lhs * lhsRatio + rhs * (1 - lhsRatio);

  // newColor.r = getAverageChar(lhs.r, lhs.r);
  // newColor.g = getAverageChar(lhs.g, lhs.g);
  // newColor.b = getAverageChar(lhs.b, lhs.b);
  // newColor.a = getAverageChar(lhs.a, lhs.a);

  return newColor;
}