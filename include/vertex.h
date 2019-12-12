#ifndef _VERTEX_H_
#define _VERTEX_H_

#include "vec3.h"
#include "color.h"

class Mat4;

struct Vertex
{
    Vec3 position = Vec3(0, 0, 0);
    Vec3 normal = Vec3(0, 0, 0);
    Color color;

    float u, v;

    Vertex();
    Vertex(const Vertex&);
    Vertex(const Vec3&);
    Vertex(const Vec3& position, const Vec3& normal);
    //Vertex(const Vec3&, Color/*, const Vec3& copiedNormal*/);
    Vertex(const Vec3&, const Vec3& copiedNormal, Color);

    void changeRef(const Mat4& transformMatrix);

    Vertex& operator=(const Vertex& vertex);
};

#endif