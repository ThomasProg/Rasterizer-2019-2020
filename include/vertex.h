#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "vec3.h"
#include "color.h"

struct Vertex
{
    Vertex();
    Vertex(const Vertex&);
    Vertex(const Vec3&);
    Vec3 position = Vec3(0, 0, 0);
    Color color;
};

#endif