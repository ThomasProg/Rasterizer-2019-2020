#include <cassert>
#include <iostream>

#include "rasterizer.h"

#include "macros.h"

#include "scene.h"
#include "texture.h"
#include "entity.h"
#include "vertex.h"
#include "mesh.h"
#include "vec3.h"
#include "vec4.h"
#include "color.h"

typedef std::vector<unsigned int>::iterator indiceIt;

void projection(Vec3 vec, int& x, int& y)
{
    x = (vec.x/2.f + 0.5f) * windowWidth;
    y = (vec.y/2.f + 0.5f) * windowHeight;

    //std::cout << x << " / " << y << std::endl;
}

void convertVertexToWindow(const Mat4& transformation, Vec3 position, int& x, int& y)
{
    Vec4 newLoc = transformation * Vec4(position, 1);
    newLoc.Homogenize();

    Vec3 newLoc3D(newLoc.x, newLoc.y, newLoc.z);
    projection(newLoc3D, x, y);
}

void Rasterizer::RenderPoints(const Entity& entity, Texture* pTarget, 
                              const std::vector<Vertex>& vertices, indiceIt& it)
{
    const Vertex& vertex = entity.mesh->vertices[*it++];

    int x, y;
    convertVertexToWindow(entity.transformation, vertex.position, x, y);

    Color c(0,0,255,0);

    pTarget->SetPixelColor(x, y, c);
}

void Rasterizer::RenderLines(const Entity& entity, Texture* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    const Vertex& vertex1 = entity.mesh->vertices[*it];
    it++;
    int x1, y1;
    convertVertexToWindow(entity.transformation, vertex1.position, x1, y1);

    const Vertex& vertex2 = entity.mesh->vertices[*it];
    it++;
    int x2, y2;
    convertVertexToWindow(entity.transformation, vertex1.position, x2, y2);

    Color c(0,0,255,0);

    for (int i = x1; i <= x2; i++)
    {
        std::cout << "draw" << std::endl;
        pTarget->SetPixelColor(0, 0, c);
    }
}

void Rasterizer::RenderTriangles(const Entity& entity, Texture* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{

}

void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->FillBlack();

    //draw triangles
    //Mesh rasterizingTriangles;
    //std::vector<>

    for (const Entity& entity : pScene->entities)
    {

        //rasterizingTriangles = entity.getTransformedTriangles();
        //for (const Vertex& vertex : entity.mesh->vertices)
        // unsigned int i = 0;
        // while (i < entity.mesh->vertices.size())
        // {
        //     i += Rasterizer::RenderPoints(entity, pTarget, i);
        // }

        std::vector<unsigned int>::iterator it = entity.mesh->indices.begin();
        while (it != entity.mesh->indices.end())
        {
            //RenderPoints(entity, pTarget, entity.mesh->vertices, it);
            RenderLines(entity, pTarget, entity.mesh->vertices, it);
        }

    }

    //Sutherrland Hardgeman algorithm
    //optimization

    //project on screen
}
