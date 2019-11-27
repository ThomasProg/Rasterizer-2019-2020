#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vec3.h"

struct Light
{
    Vec3 position = Vec3(-1.0, 0, -5.0); //y is opposite of window
    float ambientComponent  = 1.0f;
    float diffuseComponent  = 0.0f;
    float specularComponent = 0.f;

    //Light(Vec3 loc, float ambient = 0.5f, float diffuse = 0.5f, float specular = 0.5f);
};

#endif