#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "entity.h"
#include "light.h"

class Scene
{
public:
    std::vector<Entity> entities;
    std::vector<Light>  lights;

public:
    Scene();
    ~Scene();
};

#endif