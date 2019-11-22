#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vec3.h"

class Light
{
    Vec3 position;
    float ambientComponent;
    float diffuseComponent;
    float specularComponent;
};

#endif