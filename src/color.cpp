#include "color.h"

Color::Color()
    : r(0),
      g(0),
      b(0),
      a(1)
{

}

Color::Color(unsigned char r, unsigned char g, unsigned char b)
  : r(r), g(g), b(g), a(255) //255 = char max
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
}