#include "vertex.h"
#include "mat4.h"
#include "vec4.h"

Vertex::Vertex()
{

}

Vertex::Vertex(const Vertex& copied)
    : position(copied.position),
      normal(copied.normal),
      color(copied.color),
      u(copied.u),
      v(copied.v)
{

}

Vertex::Vertex(const Vec3& copiedLocation)
    : position(copiedLocation)
      //normal(copied.normal) //TODO : set normal
{

}

Vertex::Vertex(const Vec3& position, const Vec3& normal)
    : position(position),
      normal(normal.getNormalized()),
      color(0,0,0)
{

}

Vertex::Vertex(const Vec3& copiedLocation, const Vec3& copiedNormal, const Color& copiedColor)
    : position(copiedLocation),
      normal(copiedNormal),
      color(copiedColor)
{

}

Vertex& Vertex::operator=(const Vertex& vertex)
{
  position = vertex.position;
  normal = vertex.normal;
  color = vertex.color;
  u = vertex.u;
  v = vertex.v;
  return *this;
}

void Vertex::changeRef(const Mat4& transformMatrix)
{
  position = transformMatrix * Vec4(position, 1);
  normal   = transformMatrix * Vec4(normal, 0);
}