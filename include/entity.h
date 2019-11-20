#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "mat4.h"

struct Mesh;

class Entity
{
public:
    Mesh* mesh = nullptr;
    Mat4 transformation;

public:
    //Mesh getTransformedTriangles() const;
};

#endif