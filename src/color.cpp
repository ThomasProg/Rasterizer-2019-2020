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
  if (scalar < 0)
      return *this;

  else if (scalar == 0)
  {
    r = 0;
    g = 0;
    b = 0;
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