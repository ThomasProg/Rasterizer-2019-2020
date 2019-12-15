#include "glad/glad.h"
#include <GL/glu.h>
#include <GLFW/glfw3.h>
#include <queue>

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
    // {
    //     unsigned int i = 0;
    //     for (unsigned int j = 0; j < 1; j++)
    //     {
    //         Entity cube;
    //         cube.mesh = Mesh::CreateCube();
    //         i = 0;
    //         for (Vertex& vertex : cube.mesh->vertices)
    //         {
    //             if (i % 2 == 0)
    //                 vertex.color = Color(i*(1/6), 2, 2);
    //             else
    //                 vertex.color = Color(2, i*(1/6), 2);
    //             i++;
    //         }
    //         cube.transformation *= Mat4::CreateScaleMatrix(Vec3(1.0, 1.0, 1.0));
    //         //cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.9, 0, float(j)/ 1.f));
    //         cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0, 0, j*2));
    //         cube.transformation *= Mat4::CreateRotationMatrix(Vec3(0, 0.1, 0.1));
    //         cube.mesh->pTexture = &textureManager.textures[0];
    //         cube.alpha = 1.f - float(j) / 2.f;
    //         cube.alpha = 0.5f;
    //         //cube.mesh->pTexture = new Texture("media/crate.png");
    //         entities.push_back(std::move(cube));
    //     }
        


        // for (unsigned int j = 0; j < 1; j++)
        // {
        //     Entity cube;
        //     cube.mesh = Mesh::CreateCube();
        //     i = 0;
        //     for (Vertex& vertex : cube.mesh->vertices)
        //     {
        //         if (i % 2 == 0)
        //             vertex.color = Color(i*(255/6), 2, 2);
        //         else
        //             vertex.color = Color(2, i*(255/6), 2);
        //         i++;
        //     }
        //     cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0, -1, -0));
        //     cube.transformation *= Mat4::CreateScaleMatrix(Vec3(10.0, 1.0, 10.0));
        //     //cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.9, 0, float(j)/ 1.f));
        //     cube.mesh->pTexture = new Texture("media/crate2.png");
        //     entities.push_back(std::move(cube));
        //     cube.alpha = j/2.f+0.5;
        // }
    // }

    {
    //     unsigned int i = 0;
    //     for (unsigned int j = 0; j < 1; j++)
    //     {
    //         Entity cube;
    //         cube.mesh = Mesh::CreateCube();
    //         i = 0;
    //         for (Vertex& vertex : cube.mesh->vertices)
    //         {
    //             if (i % 4 == 0)
    //             {
    //                 // vertex.u = 10.f;
    //                 vertex.color = Color(1.f, 0, 0);
    //             }
    //             else
    //                 vertex.color = Color(0, 1.f, 0);
    //             i++;
    //         }
    //         //cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.9, 0, float(j)/ 1.f));
    //         cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(1, -1, j*2));
    //         //cube.transformation *= Mat4::CreateRotationMatrix(Vec3(0, 0.1, 0.1));
    //         cube.transformation *= Mat4::CreateScaleMatrix(Vec3(3.0, 3.0, 3.0));
    //         //cube.mesh->pTexture = &textureManager.textures[0];
    //         cube.alpha = 1.0f;
    //         cube.mat.additionalShaders = [](Color& color, Vec3& worldLocation)
    //         {
    //             // color.r /= (sin(worldLocation.x * 10) + PI) * 0.8; 
    //             // if (color.r > 1.f)
    //             //     color.r = 1.f;

    //             // const float delta = (sin(worldLocation.x * 10) / PI + 1) * 0.8;
    //             // color.r += (sin(worldLocation.x * 10) / PI + 1) * 0.8;
    //             // color.g += (cos(worldLocation.y * 10) / PI + 1) * 0.8;
    //             // color.b += 0;

    //             //if ((int((worldLocation.x / 1)) + int((worldLocation.y / 1)) + int((worldLocation.z / 1))) % 2 == 0)
    //             // if ((int(floor(worldLocation.x / 1)) + int(floor(worldLocation.y / 1)) + int(floor(worldLocation.z / 1))) % 2 == 0)
    //             //     color.r -= 0.2;
    //             //color.r *= 2.f;
    //             // color.g = cos(worldLocation.x * 10) * sin(worldLocation.y * 10);

    //             // if (color.r < 0)
    //             //     color.r = 0;
    //         };
    //         cube.mesh->pTexture = &textureManager.textures[0];
    //         //cube.mesh->pTexture = new Texture("media/crate.png");
    //         entities.push_back(std::move(cube));
    //     }
    for (unsigned int j = 0; j < 1; j++)
    {
        Entity sphere;
        sphere.mesh = Mesh::CreateCube();
        // float ii = 0;
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(1, 1, 1);
            //ii += 255.f / 20*20;
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-3.0, 0.0, 0.0));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
        sphere.mesh->pTexture = &textureManager.textures[0];
        sphere.alpha = 1.f;
        entities.push_back(std::move(sphere));
    }
    // {
    //     unsigned int i = 0;
    //     for (unsigned int j = 0; j < 1; j++)
    //     {
    //         Entity cube;
    //         cube.mesh = Mesh::CreateCube();
    //         i = 0;
    //         for (Vertex& vertex : cube.mesh->vertices)
    //         {
    //             if (i % 4 == 0)
    //             {
    //                 vertex.u = 10.f;
    //                 vertex.color = Color(1.f, 0, 0);
    //             }
    //             else
    //                 vertex.color = Color(0, 1.f, 0);
    //             i++;
    //         }
    //         //cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.9, 0, float(j)/ 1.f));
    //         cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0, -2, 0));
    //         //cube.transformation *= Mat4::CreateRotationMatrix(Vec3(0, 0.1, 0.1));
    //         cube.transformation *= Mat4::CreateScaleMatrix(Vec3(30, 0.1, 30));
    //         //cube.mesh->pTexture = &textureManager.textures[0];
    //         cube.alpha = 0.5f;
    //         cube.mat.additionalShaders = [](Color& color, Vec3& worldLocation)
    //         {
    //             // color.r /= (sin(worldLocation.x * 10) + PI) * 0.8; 
    //             // if (color.r > 1.f)
    //             //     color.r = 1.f;

    //             // const float delta = (sin(worldLocation.x * 10) / PI + 1) * 0.8;
    //             // color.r += (sin(worldLocation.x * 10) / PI + 1) * 0.8;
    //             // color.g += (cos(worldLocation.y * 10) / PI + 1) * 0.8;
    //             // color.b += 0;

    //             //if ((int((worldLocation.x / 1)) + int((worldLocation.y / 1)) + int((worldLocation.z / 1))) % 2 == 0)
    //             // if ((int(floor(worldLocation.x / 1)) + int(floor(worldLocation.y / 1)) + int(floor(worldLocation.z / 1))) % 2 == 0)
    //             //     color.r -= 0.2;

    //             // color.g = cos(worldLocation.x * 10) * sin(worldLocation.y * 10);

    //             // if (color.r < 0)
    //             //     color.r = 0;

    //             constexpr float div = 2;
    //             if (int(int(floor(worldLocation.x/div)) + int(floor(worldLocation.y/div)) + int(floor(worldLocation.z/div))) % 2 == 0)
    //                 color = Color(0.1, 0.1, 0.1);
    //             else 
    //                 color = Color(1.0, 1.0, 1.0);
    //         };
    //         //cube.mesh->pTexture = &textureManager.textures[1];
    //         //cube.mesh->pTexture = new Texture("media/crate.png");
    //         entities.push_back(std::move(cube));
    //     }
    // }

    // {
    //     unsigned int i = 0;
    //     for (unsigned int j = 0; j < 1; j++)
    //     {
    //         Entity cube;
    //         cube.mesh = Mesh::CreateCube();
    //         i = 0;
    //         for (Vertex& vertex : cube.mesh->vertices)
    //         {
    //             //if (i % 2 == 0)
    //                 vertex.color = Color(0.f, 1.f, 0.f);
    //             // else
    //             //     vertex.color = Color(0, 1.f, 0);
    //             i++;
    //         }
    //         cube.transformation *= Mat4::CreateScaleMatrix(Vec3(1.0, 1.0, 1.0));
    //         //cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.9, 0, float(j)/ 1.f));
    //         cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(10, 0, 3));
    //         cube.transformation *= Mat4::CreateRotationMatrix(Vec3(0, 0.1, 0.1));
    //         //cube.mesh->pTexture = &textureManager.textures[0];
    //         cube.alpha = 0.5f;
    //         //cube.mesh->pTexture = new Texture("media/crate.png");
    //         entities.push_back(std::move(cube));
    //     }
    // }
    // sphere
    // for (unsigned int j = 0; j < 1; j++)
    // {
    //     Entity sphere;
    //     sphere.mesh = Mesh::CreateSphere(15, 15);
    //     // float ii = 0;
    //     for (Vertex& vertex : sphere.mesh->vertices)
    //     {
    //         vertex.color = Color(1, 1, 1);
    //         //ii += 255.f / 20*20;
    //     }
    //     sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(3.0, 0.0, 0.0));
    //     sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(1.5, 1.5, 1.5));
    //     sphere.mesh->pTexture = &textureManager.textures[0];
    //     sphere.alpha = 1.f;
    //     entities.push_back(std::move(sphere));
    // }
    }

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

void Events::inputs(SDL_Event& event)
{
    //inputs
    while (SDL_PollEvent(&event))
    {
        switch(event.type)
        {
            case SDL_KEYDOWN:
                //cameraInputs(event.key.keysym.sym);
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
    //camera = Mat4::CreateTranslationMatrix(Vec3(0,0,10));
}

Events::~Events()
{
    
}

__inline
float lerp(float a, float b, float f)
{
    return a + f * (b - a);
}

int Events::run()
{
    if (!bRun)
        return EXIT_FAILURE;

    sceneInit(scene);

    FrameBuffer target(textureResolutionX, textureResolutionY);
    //Texture target(windowWidth, windowHeight);

    unsigned int nbFps = 0;
    float totalFps = 0.f;
    float frame = 0;
    float fps;
    float lastTime = 0.f;
    constexpr long unsigned int nbDeltaTimeSamples = 10;
    std::array<float, nbDeltaTimeSamples> deltaTimes;
    unsigned int deltaTimeIndex = 0;
    float lowestFPS = 10000;
    float highestFPS = 0.f;
    float deltaMedium = 1;

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

        float time = float (SDL_GetTicks()) / 1000.f;
        float deltaTime = time - lastTime;
        deltaTimes[deltaTimeIndex] = deltaTime;
        deltaTimeIndex++;
        if (deltaTimeIndex >= nbDeltaTimeSamples)
        {
            deltaTimeIndex = 0;

            deltaMedium = 0.f;
            for (float f : deltaTimes)
            {
                deltaMedium += f;
            }
            deltaMedium /= deltaTimes.size();
            lowestFPS = std::min(deltaMedium, lowestFPS);
            highestFPS = std::max(deltaMedium, highestFPS);
        }
        std::cout << deltaMedium << '\n';

        fps = 1.f/(deltaTime);
        nbFps++;
        totalFps += fps;
        std::cout << 1.f/(deltaTime) << std::endl;
        lastTime = time;
        //std::cout << totalFps / nbFps << std::endl;

        frame += 1;


        //scene.entities[1].transformation = Mat4::CreateTranslationMatrix(camera.cartesianLocation / 2);

        //scene.entities[1].transformation = Mat4::CreateTranslationMatrix(Vec3(camera.cartesianLocation.x, -10, camera.cartesianLocation.z));

        //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.03, 0.03, 0.03));
        // for (Entity& entity : scene.entities)
        //     scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.03, 0.03, 0.03));
            //entity.transformation = Mat4::CreateTranslationMatrix(Vec3(100 * sin(frame/50), 0.00, 0));

        //scene.entities[0].transformation = Mat4::CreateTranslationMatrix(Vec3(3.32 * sin(frame/40), 0.00, 0));

        // scene.lights[0].position.x = 10 * sin(frame/10);
        // scene.lights[0].position.y = 10 * cos(frame/10);

        scene.entities[0].mat.additionalShaders = [&deltaMedium, &highestFPS, &lowestFPS](Color& color, Vec3& worldLocation)
        {
            // color.r /= (sin(worldLocation.x * 10) + PI) * 0.8; 
            // if (color.r > 1.f)
            //     color.r = 1.f;

            // const float delta = (sin(worldLocation.x * 10) / PI + 1) * 0.8;
            // color.r += (sin(worldLocation.x * 10) / PI + 1) * 0.8;
            // color.g += (cos(worldLocation.y * 10) / PI + 1) * 0.8;
            // color.b += 0;

            //if ((int((worldLocation.x / 1)) + int((worldLocation.y / 1)) + int((worldLocation.z / 1))) % 2 == 0)
            // if ((int(floor(worldLocation.x / 1)) + int(floor(worldLocation.y / 1)) + int(floor(worldLocation.z / 1))) % 2 == 0)
            //     color.r -= 0.2;
            // color.r = (deltaMedium - lowestFPS) / (highestFPS - lowestFPS) * 1;//sin(frame/10);
            //color.r = 1 - (highestFPS - lowestFPS) / (lerp(lowestFPS, highestFPS, deltaMedium) - lowestFPS);
            //std::cout << color.r << '\n';
            color.g = cos(worldLocation.x * 10) * sin(worldLocation.y * 10);

            // if (color.r < 0)
            //     color.r = 0;
        };

        //SDL
        inputs(event);

        //GLFW
        #ifdef __GLFW__

        #ifdef __THIRD_PERSON__
        // if (glfwGetKey(window, GLFW_KEY_W))
        //     camera.spherialRadius -= camera.translationSpeed * deltaTime;

        // if (glfwGetKey(window, GLFW_KEY_S))
        //     camera.spherialRadius += camera.translationSpeed * deltaTime;
        
        // if (glfwGetKey(window, GLFW_KEY_ESCAPE))
        //     glfwSetWindowShouldClose(window, GL_TRUE);
        // if (glfwGetKey(window, GLFW_KEY_UP))
        //     camera.sphericalRotation += Vec3(camera.rotationSpeed*deltaTime,0,0);
        // if (glfwGetKey(window, GLFW_KEY_DOWN))
        //     camera.sphericalRotation += Vec3(-camera.rotationSpeed*deltaTime, 0, 0);
        // if (glfwGetKey(window, GLFW_KEY_LEFT))
        //     camera.sphericalRotation += Vec3(0,camera.rotationSpeed*deltaTime,0);
        // if (glfwGetKey(window, GLFW_KEY_RIGHT))
        //     camera.sphericalRotation += Vec3(0,-camera.rotationSpeed*deltaTime,0);
        #endif

        camera.inputs(deltaTime, window);

        f1.onSwitch = [&](bool isOn)
        {
            if (isOn)
            {
                renderMode = E_RasterizerMode::E_WIREFRAME;
            }
            else 
            {
                renderMode = E_RasterizerMode::E_TRIANGLES;
            }
        };

        if (glfwGetKey(window, GLFW_KEY_ESCAPE))
            bRun = false;

        f1.input(glfwGetKey(window, GLFW_KEY_F1));

        camera.actualize();
        
        //double xpos, ypos;
        //glfwGetCursorPos(window, &xpos, &ypos);
//
        //constexpr const double mouseSensibility = 2.f;
//
        //if (lastYPos - ypos < - mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(-0.1, 0, 0));
        //if (lastYPos - ypos > mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(0.1, 0, 0));
//
        //if (lastXPos - xpos < - mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(0.0, -0.1, 0));
        //if (lastXPos - xpos > mouseSensibility)
        //    camera *= Mat4::CreateRotationMatrix(Vec3(0.0, -0.1, 0));
//
        //lastXPos = xpos;
        //lastYPos = ypos;
        #endif


        // rasterizer
        // Rasterizer::RenderScene(&scene, &target, 
        //     Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0, 2, 1.4), 
        //     E_RasterizerMode::E_TRIANGLES);
        //Rasterizer::RenderScene(&scene, &target, Mat4::CreateOrthogonalProjectionMatrix(), camera.GetInverse(), renderMode);
        Rasterizer::RenderScene(&scene, &target, 
            Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0.05, 2, 60), 
            camera.getTransform().GetInverse(), camera, renderMode);

        // render.SDL_RenderTexture(target.texture);
        
        #ifdef __GLFW__
        #ifndef __ANTI_ALIASING__
        glDrawPixels(windowWidth, windowHeight, GL_RGBA, GL_FLOAT, target.texture.pixels);
        #else
        Texture renderedTexture(windowWidth, windowHeight);
        Rasterizer::antiAliasingCompression(target, renderedTexture);
        glDrawPixels(windowWidth, windowHeight, GL_RGBA, GL_FLOAT, renderedTexture.pixels);
        #endif
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