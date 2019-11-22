#include "sdlUtilities.h"

#include "macros.h"

#include "scene.h"
#include "entity.h"
#include "mesh.h"

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

    Entity cube;
    cube.mesh = Mesh::CreateCube();
    unsigned int i = 0;
    for (Vertex& vertex : cube.mesh->vertices)
    {
        if (i % 2 == 0)
            vertex.color = Color(i*(255/6), 0, 0);
        else
            vertex.color = Color(0, i*(255/6), 0);
        i++;
    }
    cube.transformation *= Mat4::CreateScaleMatrix(Vec3(0.9, 0.9, 0.9));
    scene.entities.push_back(cube);

    //triangle.mesh = Mesh::CreateSphere(100, 100);
    //triangle.mesh = Mesh::CreateCube();
    //triangle.mesh = Mesh::CreateTriangle(Vec3(0,0,0), Vec3(0.2,0,0), Vec3(0,0.2,0));
    // {
    // Entity triangle;
    // triangle.mesh = new Mesh;
    // triangle.mesh->vertices.push_back(Vertex(Vec3(-0.5,-0.5,0), Color(255,0,0)));
    // triangle.mesh->vertices.push_back(Vertex(Vec3(0.5,-0.5,0), Color(0,255,0)));
    // triangle.mesh->vertices.push_back(Vertex(Vec3(0.0,0.5,0), Color(0,0,255)));
    // triangle.mesh->indices.push_back(0);
    // triangle.mesh->indices.push_back(1);
    // triangle.mesh->indices.push_back(2);
    // triangle.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.0, 0, -1));
    // scene.entities.push_back(triangle);
    // }
    // {
    // Entity triangle;
    // triangle.mesh = new Mesh;
    // triangle.mesh->vertices.push_back(Vertex(Vec3(-0.5,-0.5,0), Color(255,0,0)));
    // triangle.mesh->vertices.push_back(Vertex(Vec3(0.5,-0.5,0), Color(0,255,0)));
    // triangle.mesh->vertices.push_back(Vertex(Vec3(0.0,0.5,0), Color(0,0,255)));
    // triangle.mesh->indices.push_back(0);
    // triangle.mesh->indices.push_back(1);
    // triangle.mesh->indices.push_back(2);
    // triangle.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.5, 0, -1));
    // scene.entities.push_back(triangle);
    // }
    Entity sphere;
    sphere.mesh = Mesh::CreateSphere(20, 20);
    float ii = 0;
    for (Vertex& vertex : sphere.mesh->vertices)
    {
        vertex.color = Color(i, 0, 255);
        ii += 255 / 20*20;
    }
    sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(-0.5, 0, 0));
    sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(0.8, 0.8, 0.8));
    scene.entities.push_back(sphere);

    //scene.entities[0].transformation *= Mat4::CreateScaleMatrix(Vec3(0.5, 2, 2));

    // Entity line;
    // line.mesh = new Mesh;
    // line.mesh->vertices.push_back(Vertex(Vec3(0.0,0.0,0), Color(255,0,0)));
    // line.mesh->vertices.push_back(Vertex(Vec3(1.5,-1.2,0), Color(0,255,0)));
    // line.mesh->indices.push_back(0);
    // line.mesh->indices.push_back(1);
    // scene.entities.push_back(line);

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

    SDL_Texture* sdl_texturePtr = SDL_CreateTexture(render.renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

    while (bRun)
    {
        float time = float (SDL_GetTicks()) / 1000.f;
        fps = 1.f/(time - lastTime);
        nbFps++;
        totalFps += fps;
        lastTime = time;
        std::cout << totalFps / nbFps << std::endl;
        //triangle.mesh->vertices[1].color.b = 255;
        //std::cout << (int)triangle.mesh->vertices[0].color.r << std::endl;
        frame += 1;
        //std::cout << (int)scene.entities[0].mesh->vertices[0].color.r << std::endl;

        scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.01, 0.01, 0.01));
        //scene.entities[0].transformation *= Mat4::CreateTranslationMatrix(Vec3(0, 0, -frame));
        //std::cout << scene.entities[0].transformation << std::endl;
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

                default:break;
            }
        }

        //rasterizer
        Rasterizer::RenderScene(&scene, &target);

        target.texture.ToTexture(sdl_texturePtr);
        SDL_RenderClear(render.renderer);
        SDL_RenderCopy(render.renderer, sdl_texturePtr, nullptr, nullptr);
        //SDL_Utilities::textureToSDL(target.texture, render.renderer);
        SDL_RenderPresent(render.renderer);
    }

    SDL_DestroyTexture(sdl_texturePtr);

    return EXIT_SUCCESS;
}
