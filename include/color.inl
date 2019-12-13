#include <cassert>
#include "color.h"

__inline
Color::Color()
    : r(0),
      g(0),
      b(0),
      a(1)
{

}

__inline
Color::Color(float r, float g, float b)
  : r(r), g(g), b(b), a(1.f)
{

}

__inline
Color::Color(float r, float g, float b, float a)
  : r(r), g(g), b(b), a(a)
{

}

__inline
Color::Color(const Color& setColor)
  : r(setColor.r), g(setColor.g), b(setColor.b), a(setColor.a)
{

}

__inline
Color& Color::operator=(const Color& setColor) noexcept
{
  r = setColor.r;
  g = setColor.g;
  b = setColor.b;
  a = setColor.a;
  return *this;
}

__inline
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

__inline
Color  Color::operator*(float scalar) const noexcept
{
  if (scalar <= 0)
  {
    return Color(0, 0, 0, 0);
  }
  else
    return Color(r*scalar, g*scalar, b*scalar, a*scalar);
}

__inline
Color  Color::operator+(const Color& rhs) const noexcept
{
  return Color(r+rhs.r, g+rhs.g, b+rhs.b, a+rhs.a);
}

__inline
Color  Color::operator/(unsigned char f) const noexcept
{
  assert(f != 0);
  return Color(r/f, g/f, b/f, a/f);
}

__inline
Color& Color::operator+=(const Color& rhs) noexcept
{
  r += rhs.r;
  g += rhs.g;
  b += rhs.b;
  a += rhs.a;

  return *this;
}

__inline
float Color::getTransparence() const noexcept
{
  return this->a;
}

__inline
void Color::copyRGB(const Color& rhs) noexcept
{
  r = rhs.r;
  g = rhs.g;
  b = rhs.b;
}

__inline
void Color::reset() noexcept
{
    r = 0;
    g = 0;
    b = 0;
}

__inline
Color getAverageColor(const Color& lhs, const Color& rhs, float lhsRatio) noexcept
{
  return lhs * lhsRatio + rhs * (1 - lhsRatio);
}
