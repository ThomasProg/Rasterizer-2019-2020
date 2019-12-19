#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "mat4.h"
#include "vec3.h"
#include "material.h"

struct Mesh;

class Entity
{
public:
    Mesh* mesh = nullptr;
    Mat4 transformation;

    float alpha = 1.0f; //entity transparency, [0;1]
    Material mat;

    // prevent computation of center 
    // everytime we compare entities location for alpha blending
    Vec3 center;

    void computeCenter();
};

#endif