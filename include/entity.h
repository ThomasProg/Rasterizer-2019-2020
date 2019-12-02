#ifndef _ENTITY_H_
#define _ENTITY_H_

#include "mat4.h"

struct Mesh;

class Entity
{
public:
    Mesh* mesh = nullptr;
    Mat4 transformation;

    //bugs if alpha < 0.3
    float alpha = 1.0f; //entity transparency, [0;1]
};

#endif