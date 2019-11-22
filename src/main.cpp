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

    // Entity cube;
    // cube.mesh = Mesh::CreateCube();
    // unsigned int i = 0;
    // for (Vertex& vertex : cube.mesh->vertices)
    // {
    //     vertex.color = Color(255, 0, 0);
    //     i++;
    // }
    // cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(-0.5, 0, 2));
    // scene.entities.push_back(cube);

    //triangle.mesh = Mesh::CreateSphere(100, 100);
    //triangle.mesh = Mesh::CreateCube();
    //triangle.mesh = Mesh::CreateTriangle(Vec3(0,0,0), Vec3(0.2,0,0), Vec3(0,0.2,0));
    // triangle.mesh = new Mesh;
    // triangle.mesh->vertices.push_back(Vertex(Vec3(-0.5,-0.5,0), Color(255,0,0)));
    // triangle.mesh->vertices.push_back(Vertex(Vec3(0.5,-0.5,0), Color(0,255,0)));
    // triangle.mesh->vertices.push_back(Vertex(Vec3(0.0,0.5,0), Color(0,0,255)));
    // triangle.mesh->indices.push_back(0);
    // triangle.mesh->indices.push_back(1);
    // triangle.mesh->indices.push_back(2);


    // Entity sphere;
    // sphere.mesh = Mesh::CreateSphere(10, 10);
    // for (Vertex& vertex : sphere.mesh->vertices)
    // {
    //     vertex.color = Color(0, 0, 255);
    // }
    // sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.5, 0, 2));
    // scene.entities.push_back(sphere);

    //scene.entities[0].transformation *= Mat4::CreateScaleMatrix(Vec3(0.5, 2, 2));

    Entity line;
    line.mesh = new Mesh;
    line.mesh->vertices.push_back(Vertex(Vec3(0.0,0.0,0), Color(255,0,0)));
    line.mesh->vertices.push_back(Vertex(Vec3(1.5,-1.2,0), Color(0,255,0)));
    line.mesh->indices.push_back(0);
    line.mesh->indices.push_back(1);
    scene.entities.push_back(line);

    FrameBuffer target(windowWidth, windowHeight);
    //Texture target(windowWidth, windowHeight);

    float frame = 0;

    bool bRun = true;
    while (bRun)
    {
        //triangle.mesh->vertices[1].color.b = 255;
        //std::cout << (int)triangle.mesh->vertices[0].color.r << std::endl;
        frame += 1;
        //std::cout << (int)scene.entities[0].mesh->vertices[0].color.r << std::endl;

        //scene.entities[0].transformation *= Mat4::CreateRotationMatrix(Vec3(0.09, 0.09, 0.09));
        //scene.entities[0].transformation *= Mat4::CreateTranslationMatrix(Vec3(0, 0, sin(frame) * 0.1));
        //std::cout << scene.entities[0].transformation << std::endl;
        //inputs
        SDL_Event event;
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

        SDL_RenderClear(render.renderer);
        SDL_Utilities::textureToSDL(target.texture, render.renderer);
        SDL_RenderPresent(render.renderer);
    }

    return EXIT_SUCCESS;
}
