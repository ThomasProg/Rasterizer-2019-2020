#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(const Vertex& copied)
    : position(copied.position),
      color(copied.color)
{

}

Vertex::Vertex(const Vec3& copiedLocation)
    : position(copiedLocation)
{

}

Vertex::Vertex(const Vec3& copiedLocation, Color copiedColor)
    : position(copiedLocation),
      color(copiedColor)
{

}