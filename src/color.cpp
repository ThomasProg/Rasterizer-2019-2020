#include "color.h"

Color::Color()
    : r(0),
      g(0),
      b(0),
      a(1)
{

}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
  : r(r), g(g), b(b), a(255) //255 = char max
{

}

Color::Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
  : r(r), g(g), b(b), a(a)
{

}

Color::Color(const Color& setColor)
    : r(setColor.r),
      g(setColor.g),
      b(setColor.b),
      a(setColor.a)
{

}

Color& Color::operator=(Color setColor)
{
    r = setColor.r;
    g = setColor.g;
    b = setColor.b;
    a = setColor.a;

    return *this;
}

Color& Color::operator*=(float scalar)
{
  if (scalar <= 0)
  {
    r = 0;
    g = 0;
    b = 0;
    return *this;
  }

  //since -1 < intensity < 1, we need to lerp the intensity for the color
  if (r > 255.f / scalar)
      r = 255;
  else 
      r *= scalar;
      
  if (g > 255.f / scalar)
      g = 255;
  else
      g *= scalar;

  if (b > 255.f / scalar)
      b = 255;       
  else 
      b *= scalar;  

  return *this;
}

unsigned char& Color::operator[](unsigned int index)
{
  switch (index)
  {
  case 0:
    return r;
    break;

  case 1:
    return g;
    break;
  
  case 2:
    return b;
    break;

  case 3:
    return a;
    break;

  default:
    break;
  }
}

Color Color::operator*(float scalar)
{
  Color c = *this;
  if (scalar <= 0)
  {
    c.r = 0;
    c.g = 0;
    c.b = 0;

    return c;
  }

  #define multiplyAndClampChar(result, c, scalar) \
    if (c > 255 / scalar)\
      result = 255;        \
    else                \
      result = c * scalar;

  multiplyAndClampChar(c.r, this->r, scalar) 
  multiplyAndClampChar(c.g, this->g, scalar) 
  multiplyAndClampChar(c.b, this->b, scalar) 

  //TODO : TODELETE

  // //since -1 < intensity < 1, we need to lerp the intensity for the color
  // if (r > 255.f / scalar)
  //     c.r = 255;
  // else 
  //     c.r *= scalar;
      
  // if (g > 255.f / scalar)
  //     c.g = 255;
  // else
  //     c.g *= scalar;

  // if (b > 255.f / scalar)
  //     c.b = 255;       
  // else 
  //     c.b *= scalar;  

  #undef multiplyAndClampChar

  return c;
}

Color Color::operator+(const Color& rhs)
{
  Color c;
  #define addAndClampChar(result, f1, f2) \
    if (f1 > 255 - f2)\
      result = 255;        \
    else                \
      result = f1 + f2;

  addAndClampChar(c.r, this->r, rhs.r);
  addAndClampChar(c.g, this->g, rhs.g);
  addAndClampChar(c.b, this->b, rhs.b);

  #undef addAndClampChar

  return c;
}

float Color::getTransparence() const
{
  return float(this->a) / 255.f;
}

Color getAverageColor(Color lhs, Color rhs, float lhsRatio)
{
  Color newColor = lhs * lhsRatio + rhs * (1 - lhsRatio);

  // newColor.r = getAverageChar(lhs.r, lhs.r);
  // newColor.g = getAverageChar(lhs.g, lhs.g);
  // newColor.b = getAverageChar(lhs.b, lhs.b);
  // newColor.a = getAverageChar(lhs.a, lhs.a);

  return newColor;
}