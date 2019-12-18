#include <cassert>

#include "entity.h"
#include "vertex.h"
#include "mesh.h"

void Entity::computeCenter()
{
    if (mesh != nullptr)
    {
        for (const Vertex& vertex : mesh->worldVertices)
        {
            center += vertex.position;
        }

        center /= mesh->worldVertices.size();
    }
}