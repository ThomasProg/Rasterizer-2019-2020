#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "vec3.h"
#include "color.h"

struct Vertex
{
    Vec3 position = Vec3(0, 0, 0);
    Vec3 normal = Vec3(0, 0, 0);
    Color color;

    Vertex();
    Vertex(const Vertex&);
    Vertex(const Vec3&);
    Vertex(const Vec3&, Color/*, const Vec3& copiedNormal*/);
};

#endif