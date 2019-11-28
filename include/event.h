#ifndef _EVENT_H_
#define _EVENT_H_

#include <SDL2/SDL.h>
#include <vector>

#include "mat4.h"
#include "scene.h"
#include "sdlUtilities.h"

#include "rasterizer.h"

#include "doOnce.h"

class Light;
class Scene;
class Entity;

class Events
{
private:

    //inputs
    SDL_Event event;
    bool bRun;
    Mat4 camera;

    SDL_Utilities render;
    Scene scene;

    E_RasterizerMode renderMode = E_RasterizerMode::E_TRIANGLES;

    DoOnce F1;

public:
    Events();
    ~Events();

    void lightsInit(std::vector<Light>& lights);
    void entitiesInit(std::vector<Entity>& entities);
    void sceneInit(Scene& scene);

    void inputs(SDL_Event& event, bool& bRun);

    void cameraInputs(int touch);
    int run();
};


#endif