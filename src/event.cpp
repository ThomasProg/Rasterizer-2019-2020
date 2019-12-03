#include "glad/glad.h"
#include <GL/glu.h>
#include <GLFW/glfw3.h>

#include "event.h"

#include "sdlUtilities.h"

#include "vertex.h"
#include "mesh.h"
#include "light.h"

#include "macros.h"

#include "scene.h"
#include "entity.h"
#include "event.h"

#include "texture.h"
#include "frameBuffer.h"

#include "rasterizer.h"
#include "color.h"

void Events::lightsInit(std::vector<Light>& lights)
{
    lights.push_back(Light());
}

void Events::entitiesInit(std::vector<Entity>& entities)
{
    // //cube
    {
        unsigned int i = 0;
        for (unsigned int j = 0; j < 1; j++)
        {
            Entity cube;
            cube.mesh = Mesh::CreateCube();
            i = 0;
            for (Vertex& vertex : cube.mesh->vertices)
            {
                if (i % 2 == 0)
                    vertex.color = Color(i*(255/6), 2, 2);
                else
                    vertex.color = Color(2, i*(255/6), 2);
                i++;
            }
            cube.transformation *= Mat4::CreateScaleMatrix(Vec3(1.0, 1.0, 1.0));
            //cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.9, 0, float(j)/ 1.f));
            cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0, 2, -10));
            cube.mesh->pTexture = new Texture("media/crate.png");
            entities.push_back(std::move(cube));
            cube.alpha = j/2.f+0.5;
        }
    }

    // // sphere
    // for (unsigned int j = 0; j < 1; j++)
    // {
    //     Entity sphere;
    //     sphere.mesh = Mesh::CreateSphere(15, 15);
    //     // float ii = 0;
    //     for (Vertex& vertex : sphere.mesh->vertices)
    //     {
    //         vertex.color = Color(0, 10, 255);
    //         //ii += 255.f / 20*20;
    //     }
    //     sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.0, 0.0, 0.0));
    //     sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(0.05, 0.05, 0.05));
    //     entities.push_back(std::move(sphere));
    // }

    // {
    //     Entity triangle;
    //     triangle.mesh = Mesh::CreateTriangle(Vec3(-0.5,0.5,0.5), Vec3(0.5,0.5,0.0), Vec3(0.5,-0.5,0.5));
    //     //triangle.mesh = Mesh::CreateCube();
    //     for (Vertex& vertex : triangle.mesh->vertices)
    //         vertex.color = Color(255, 0, 0);
    //     // triangle.mesh->vertices[0].color = Color(255, 0, 0);
    //     // triangle.mesh->vertices[1].color = Color(0, 255, 0);
    //     // triangle.mesh->vertices[2].color = Color(0, 0, 255);

    //     entities.push_back(std::move(triangle));
    // }
}

void Events::sceneInit(Scene& scene)
{
    lightsInit(scene.lights);
    entitiesInit(scene.entities);
}

void Events::cameraInputs(int touch)
{
    // if (touch == SDLK_UP || touch == SDLK_w)
    //     camera = Mat4::CreateTranslationMatrix(Vec3(0, 0, 1)) * camera;

    // if (touch == SDLK_DOWN || touch == SDLK_s)
    //     camera = Mat4::CreateTranslationMatrix(Vec3(0, 0, -1)) * camera;

    // if (touch == SDLK_RIGHT || touch == SDLK_d)
    //     camera = Mat4::CreateTranslationMatrix(Vec3(1, 0, 0)) * camera;

    // if (touch == SDLK_DOWN || touch == SDLK_a)
    //     camera = Mat4::CreateTranslationMatrix(Vec3(-1, 0, 0)) * camera;
}

void Events::inputs(SDL_Event& event, bool& bRun)
{
    //inputs
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                cameraInputs(event.key.keysym.sym);
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE :
                        bRun = false;
                        break;
                    case SDLK_F1 :
                        //F1.input(true);
                        renderMode = E_RasterizerMode::E_WIREFRAME;
                        break;
                    
                }
                break;

            default: break;
        }
    }
}

Events::Events()
#ifdef __SDL__
    : render(SDL_Utilities(bRun))
#endif
{
    // F1.onSwitch = [&](bool isOn)
    // {
    //     if (isOn)
    //         renderMode = E_RasterizerMode::E_WIREFRAME;
    //     else
    //         renderMode = E_RasterizerMode::E_TRIANGLES;
    // };

    #ifdef __GLFW__
    if (!glfwInit())
    {
        bRun = false;
        return;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);

    window = glfwCreateWindow(windowWidth, windowHeight, "Geometry", NULL, NULL);

    glfwMakeContextCurrent(window);

    if (!gladLoadGL())
    {
        glfwTerminate();
        bRun = false;
        return;
    }

    //hide cursor
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    #endif
}

Events::~Events()
{
    
}

int Events::run()
{
    if (!bRun)
        return EXIT_FAILURE;

    sceneInit(scene);

    FrameBuffer target(windowWidth, windowHeight);
    //Texture target(windowWidth, windowHeight);

    unsigned int nbFps = 0;
    float totalFps = 0.f;
    float frame = 0;
    float fps;
    float lastTime = 0.f;
    //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.00, 0.00, PI/5));
    #ifdef __SDL__
    while (bRun)
    #endif
    #ifdef __GLFW__
    while (bRun && !glfwWindowShouldClose(window))
    #endif
    {
        #ifdef __GLFW__
        glfwPollEvents();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        #endif

        Mat4 m = Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0.05, 2, 90);
        std::cout << m << std::endl;

        float time = float (SDL_GetTicks()) / 1000.f;
        fps = 1.f/(time - lastTime);
        nbFps++;
        totalFps += fps;
        std::cout << 1.f/(time - lastTime) << std::endl;
        lastTime = time;
        //std::cout << totalFps / nbFps << std::endl;

        frame += 1;

        //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.09, 0.09, 0.09));
        //scene.entities[1].transformation *= Mat4::CreateTranslationMatrix(Vec3(0.00, 0.00, 10 * sin(frame/10)));
        scene.lights[0].position.x = 10 * sin(frame/10);
        scene.lights[0].position.y = 10 * cos(frame/10);

        //SDL
        inputs(event, bRun);

        //GLFW
        #ifdef __GLFW__
        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            glfwSetWindowShouldClose(window, GL_TRUE);
        if (glfwGetKey(window, GLFW_KEY_UP))
            camera *= Mat4::CreateTranslationMatrix(Vec3(0, 0, 0.1));
        if (glfwGetKey(window, GLFW_KEY_DOWN))
            camera *= Mat4::CreateTranslationMatrix(Vec3(0, 0, -0.1));
        if (glfwGetKey(window, GLFW_KEY_LEFT))
            camera *= Mat4::CreateTranslationMatrix(Vec3(-0.1, 0, 0));
        if (glfwGetKey(window, GLFW_KEY_RIGHT))
            camera *= Mat4::CreateTranslationMatrix(Vec3(0.1, 0, 0));

        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        constexpr const double mouseSensibility = 2.f;

        if (lastYPos - ypos < - mouseSensibility)
            camera *= Mat4::CreateRotationMatrix(Vec3(-0.1, 0, 0));
        if (lastYPos - ypos > mouseSensibility)
            camera *= Mat4::CreateRotationMatrix(Vec3(0.1, 0, 0));

        if (lastXPos - xpos < - mouseSensibility)
            camera *= Mat4::CreateRotationMatrix(Vec3(0.0, -0.1, 0));
        if (lastXPos - xpos > mouseSensibility)
            camera *= Mat4::CreateRotationMatrix(Vec3(0.0, -0.1, 0));

        lastXPos = xpos;
        lastYPos = ypos;
        #endif


        // rasterizer
        // Rasterizer::RenderScene(&scene, &target, 
        //     Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0, 2, 1.4), 
        //     E_RasterizerMode::E_TRIANGLES);
        //Rasterizer::RenderScene(&scene, &target, Mat4::CreateOrthogonalProjectionMatrix(), camera.GetInverse(), renderMode);
        Rasterizer::RenderScene(&scene, &target, 
            Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0.05, 2, 90), 
            camera.GetInverse(), renderMode);

        // render.SDL_RenderTexture(target.texture);

        #ifdef __GLFW__
        glDrawPixels(windowWidth, windowHeight, GL_RGBA, GL_UNSIGNED_BYTE, target.texture.pixels);
        glfwSwapBuffers(window);
        #endif

        #ifdef __SDL__
        SDL_RenderClear(render.renderer);
        render.SDL_RenderTexture(target.texture);

        SDL_RenderPresent(render.renderer);
        #endif
    }

    #ifdef __GLFW__
    glfwDestroyWindow(window);
    glfwTerminate();
    #endif

    return EXIT_SUCCESS;
}