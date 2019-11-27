#include "sdlUtilities.h"

#include "macros.h"

#include "scene.h"
#include "entity.h"
#include "mesh.h"
#include "event.h"

#include "texture.h"
#include "frameBuffer.h"

#include "rasterizer.h"
#include "color.h"

int main(int argc, char** argv)
{
    bool isValid;
    SDL_Utilities render(isValid);
    if (!isValid)
        return EXIT_FAILURE;


    Scene scene;
    sceneInit(scene);

    FrameBuffer target(windowWidth, windowHeight);
    //Texture target(windowWidth, windowHeight);

    unsigned int nbFps = 0;
    float totalFps = 0.f;
    float frame = 0;
    float fps;
    float lastTime = 0.f;
    //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.00, 0.00, PI/5));
    bool bRun = true;
    SDL_Event event;

    while (bRun)
    {
        float time = float (SDL_GetTicks()) / 1000.f;
        fps = 1.f/(time - lastTime);
        nbFps++;
        totalFps += fps;
        lastTime = time;
        std::cout << totalFps / nbFps << std::endl;

        frame += 1;

        //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));
        //scene.entities[1].transformation *= Mat4::CreateTranslationMatrix(Vec3(0.00, 0.00, 10 * sin(frame/10)));
        // scene.lights[0].position.x = 10 * sin(frame/10);
        // scene.lights[0].position.y = 10 * cos(frame/10);

        //inputs
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE :
                            bRun = false;
                            break;
                    }

                default: break;
            }
        }

        // // rasterizer
        // Rasterizer::RenderScene(&scene, &target, 
        //     Mat4::CreatePerspectiveProjectionMatrix(windowWidth, windowHeight, 0, 2, 1.4), 
        //     E_RasterizerMode::E_TRIANGLES);
        Rasterizer::RenderScene(&scene, &target, Mat4::CreateOrthogonalProjectionMatrix(), E_RasterizerMode::E_TRIANGLES);


        render.SDL_RenderTexture(target.texture);
    }

    return EXIT_SUCCESS;
}
