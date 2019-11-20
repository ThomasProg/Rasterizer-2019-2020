#ifndef _SCENE_H_
#define _SCENE_H_

#include <vector>
#include "entity.h"

class Scene
{
public:
    std::vector<Entity> entities;

public:
    Scene();
    ~Scene();
};

#endif