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

Vertex::Vertex(const Vec3& position, const Vec3& normal)
    : position(position),
      normal(normal.getNormalized()),
      color(0,0,0)
{

}

// Vertex::Vertex(const Vec3& copiedLocation, Color copiedColor/*, const Vec3& copiedNormal*/)
//     : position(copiedLocation),
//       //normal(copiedNormal),
//       color(copiedColor)
// {

// }

Vertex::Vertex(const Vec3& copiedLocation, const Vec3& copiedNormal, Color copiedColor)
    : position(copiedLocation),
      normal(copiedNormal),
      color(copiedColor)
{

}

