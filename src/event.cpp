#include "event.h"

#include "vertex.h"
#include "mesh.h"
#include "light.h"
#include "scene.h"

void lightsInit(std::vector<Light>& lights)
{
    lights.push_back(Light());
}

void entitiesInit(std::vector<Entity>& entities)
{
    //cube
    {
        unsigned int i = 0;
        //for (unsigned int j = 0; j < 20; j++)
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
            cube.transformation *= Mat4::CreateTranslationMatrix(Vec3(-1.0, 0, 0));
            cube.transformation *= Mat4::CreateScaleMatrix(Vec3(0.9, 0.9, 0.9));
            entities.push_back(std::move(cube));
        }
    }

    //sphere
    {
        Entity sphere;
        sphere.mesh = Mesh::CreateSphere(40, 40);
        float ii = 0;
        for (Vertex& vertex : sphere.mesh->vertices)
        {
            vertex.color = Color(0, 10, 255);
            ii += 255.f / 20*20;
        }
        sphere.transformation *= Mat4::CreateTranslationMatrix(Vec3(0.5, 0.0, 0.0));
        sphere.transformation *= Mat4::CreateScaleMatrix(Vec3(0.9, 0.9, 0.9));
        entities.push_back(std::move(sphere));
    }
}

void sceneInit(Scene& scene)
{
    lightsInit(scene.lights);
    entitiesInit(scene.entities);
}


void inputs(SDL_Event& event)
{
    
}