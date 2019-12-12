#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "mat4.h"
#include "vec3.h"

struct Mesh;

class Entity
{
public:
    Mesh* mesh = nullptr;
    Mat4 transformation;

    //bugs if alpha < 0.3
    float alpha = 1.0f; //entity transparency, [0;1]

    // prevent computation of center 
    // everytime we compare entities location for alpha blending
    Vec3 center;

    void computeCenter();
};

#endif