#include "vertex.h"

Vertex::Vertex()
{

}

Vertex::Vertex(const Vertex& copied)
    : position(copied.position)
{

}

Vertex::Vertex(const Vec3& copiedLocation)
    : position(copiedLocation)
{

}