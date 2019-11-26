#ifndef _EVENT_H_
#define _EVENT_H_

#include <SDL2/SDL.h>
#include <vector>

class Light;
class Scene;
class Entity;

void lightsInit(std::vector<Light>& lights);
void entitiesInit(std::vector<Entity>& entities);
void sceneInit(Scene& scene);

void inputs(SDL_Event& event);


#endif