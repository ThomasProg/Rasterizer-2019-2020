#include <set>

#include "scene.h"
#include "mesh.h"

Scene::Scene()
{

}

Scene::~Scene()
{
    //get each mesh pointer only once
    std::set<Mesh*> meshes;
    for (const Entity& entity : entities)
    {
        meshes.emplace(entity.mesh);
    }

    //delete each pointer once
    for (Mesh* mesh : meshes)
    {
        delete mesh;
    }
}
