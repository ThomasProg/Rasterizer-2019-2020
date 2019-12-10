#ifndef _EVENT_H_
#define _EVENT_H_

#include <SDL2/SDL.h>
#include <vector>

#include <GLFW/glfw3.h>

#include "mat4.h"
#include "scene.h"
#include "sdlUtilities.h"

#include "rasterizer.h"

#include "camera.h"

#include "macros.h"
#include "doOnce.h"
#include "ressourceManager.hpp"

class Light;
class Scene;
class Entity;

class Events
{
private:

    //inputs
    SDL_Event event;
    bool bRun = true;
    Camera camera;

    double lastXPos = 0, lastYPos = 0;

    #ifdef __SDL__
    SDL_Utilities render;
    #endif

    #ifdef __GLFW__
    GLFWwindow* window;
    #endif
    
    Scene scene;
    RessourceManager textureManager;

    E_RasterizerMode renderMode = E_RasterizerMode::E_TRIANGLES;

    DoOnce f1;
    //bool f1;

public:
    Events();
    ~Events();

    void lightsInit(std::vector<Light>& lights);
    void entitiesInit(std::vector<Entity>& entities);
    void sceneInit(Scene& scene);

    void inputs(SDL_Event& event);

    int run();
};


#endif