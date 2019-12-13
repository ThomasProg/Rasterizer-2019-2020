#include "color.h"

std::ostream& operator<<(std::ostream& stream, const Color& c)
{
  stream << c.r << ' ' << c.g << ' ' << c.b << ' ' << c.a << '\n';
  return stream;
}