#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vec3.h"

struct Light
{
    Vec3 position = Vec3(5.0, 0, 10.0); //y is opposite of window

    float ambientComponent  = 0.2f;
    float diffuseComponent  = 0.7f;
    float specularComponent = 0.5f;

    //Light(Vec3 loc, float ambient = 0.5f, float diffuse = 0.5f, float specular = 0.5f);
};

#endif