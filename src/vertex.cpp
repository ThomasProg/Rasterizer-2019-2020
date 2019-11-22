#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(const Vertex& copied)
    : position(copied.position),
      normal(copied.normal),
      color(copied.color)
{

}

Vertex::Vertex(const Vec3& copiedLocation)
    : position(copiedLocation)
      //normal(copied.normal) //TODO : set normal
{

}

Vertex::Vertex(const Vec3& copiedLocation, Color copiedColor/*, const Vec3& copiedNormal*/)
    : position(copiedLocation),
      //normal(copiedNormal),
      color(copiedColor)
{

}