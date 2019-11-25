#include <cassert>
#include <cmath>
#include <iostream>
#include <functional>
#include <array>
#include <cassert>

#include "rasterizer.h"

#include "macros.h"

#include "scene.h"
#include "texture.h"
#include "frameBuffer.h"

#include "entity.h"
#include "vertex.h"
#include "vertex2.h"
#include "rasterizingVertex.h"
#include "mesh.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "color.h"

#include "renderTriangle.h"

typedef std::vector<unsigned int>::iterator indiceIt;

Vec3 Rasterizer::projection(const Vec3& vec)
{
    return Vec3(((vec.x/5) + 1) * 0.5 * windowWidth, windowHeight - ((vec.y/5) + 1) * 0.5 * windowHeight, vec.z);

    // return Vec3((vec.x/2.f + 0.5f) * windowWidth,
    //             (vec.y/2.f + 0.5f) * windowHeight, vec.z);
}

/* 
 * if used, draw vertices as points
 * Complexity : O(1)
 */
void Rasterizer::RenderPoints(FrameBuffer* pTarget, 
                              std::vector<Vertex>& vertices, indiceIt& it)
{
    const Vertex& vertex = vertices[*it++];

    //Vertex2 screenVertex = convertVertexToWindow(entity.transformation, vertex);

    pTarget->SetPixel(vertex.position.x, vertex.position.y, vertex.position.z, vertex.color);
}

void drawLine(FrameBuffer* pTarget, const Vertex& vertex1, const Vertex& vertex2)
{
    //get distance between two points
    const int& x1 = vertex1.position.x;
    const int& y1 = vertex1.position.y;
    const int& x2 = vertex2.position.x;
    const int& y2 = vertex2.position.y;
    
    Vec2 vec2(x2-x1, y2-y1);
    //get distance between the 2 points
    float magnitude = vec2.GetMagnitude();

    if (magnitude != 0.f)
    {
        vec2.toUnit();

        for (float i = 0; i <= magnitude; ++i)
        {
            float ratio = i / magnitude;
            Vec3 point(x1 + i * vec2.x, 
                       y1 + i * vec2.y, 
                       vertex1.position.z * ratio + vertex2.position.z * (1 - ratio));
            //std::cout << "z : " << point.z << std::endl;

            float weight1 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
            float weight2 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
            
            //Color c(vertex1.color.r, vertex1.color.g, vertex1.color.b);
            Color c(255,255,255);

            pTarget->SetPixel(point.x, point.y, point.z, c);
        }
    }
    else 
        pTarget->SetPixel(x2, y2, vertex2.position.z, vertex1.color);
}

/* 
 * if used, draw vertices as lines
 * Complexity : O(n), n being the distance on screen between the 2 points
 */
void Rasterizer::RenderLines(FrameBuffer* pTarget, 
                            std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    //get first point on screen
    const Vertex& vertex1 = vertices[*it++];
    const Vertex& vertex2 = vertices[*it++];

    drawLine(pTarget, vertex1, vertex2);
}

void Rasterizer::RenderTriangles(FrameBuffer* pTarget, 
                            std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    // std::array<Vertex*, 3> triangleVertices3D;
    // std::array<Vertex, 3> triangleVertices2D;

    std::array<RasterizingVertex, 3> triangleVertices;
    // triangleVertices[1].setFromVertex(vertices[1], projection);
    // triangleVertices[2].setFromVertex(vertices[2], projection);

    //get first point on screen
    triangleVertices[0].setFromVertex(vertices[*it++], Rasterizer::projection);

    triangleVertices[1].setFromVertex(vertices[*it++], Rasterizer::projection);

    triangleVertices[2].setFromVertex(vertices[*it++], Rasterizer::projection);


    //draw normals
    Vec3 n1 = Rasterizer::projection(triangleVertices[0].normal*(1) + triangleVertices[0].position3D);
    Vec3 n2 = Rasterizer::projection(triangleVertices[1].normal*(1) + triangleVertices[1].position3D);
    Vec3 n3 = Rasterizer::projection(triangleVertices[2].normal*(1) + triangleVertices[2].position3D);
    
    drawLine(pTarget, triangleVertices[0].position2D, n1);
    drawLine(pTarget, triangleVertices[1].position2D, n2);
    drawLine(pTarget, triangleVertices[2].position2D, n3);


    // //wireframe mode
    drawLine(pTarget, triangleVertices[0].position2D, triangleVertices[1].position2D);
    drawLine(pTarget, triangleVertices[1].position2D, triangleVertices[2].position2D);
    drawLine(pTarget, triangleVertices[2].position2D, triangleVertices[0].position2D);

    //sort
    // if (triangleVertices[0]->position.y > triangleVertices[1]->position.y)
    //     std::swap(triangleVertices[0], triangleVertices[1]);
    // if (triangleVertices[1]->position.y > triangleVertices[2]->position.y)
    //     std::swap(triangleVertices[1], triangleVertices[2]);
    // if (triangleVertices[0]->position.y > triangleVertices[1]->position.y)
    //     std::swap(triangleVertices[0], triangleVertices[1]);

    RenderTriangle tri(triangleVertices, pTarget);
    tri.draw();
}

// /////////////////////////////////////////////////
// ///////////////////END///////////////////////
// /////////////////////////////////////////////////

// void addClippedVertex(const Vec4& vert,
//                       Color color,
//                       std::vector<Vertex>& transformedVertices, 
//                       std::vector<unsigned int>& transformedIndices)
void addClippedVertex(const Vec4& vert,
                      Color color,
                      const Vec4& normal,
                      std::vector<Vertex>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    transformedVertices.emplace_back(Vertex(vert.getHomogenizedVec(), normal, color));
    transformedIndices.emplace_back(transformedVertices.size() - 1);
}

//vertices size should be 4
void add4ClippedVertex(const Vec4* vertices,
                      const Color* color,
                      const Vec4* normal,
                      std::vector<Vertex>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    for (unsigned int i = 0; i < 4; i++)
    {
        transformedVertices.push_back(Vertex(Rasterizer::projection(vertices[i].getHomogenizedVec()), normal[i], color[i]));
    }
    
    transformedIndices.push_back(transformedVertices.size() - 4);
    transformedIndices.push_back(transformedVertices.size() - 3);
    transformedIndices.push_back(transformedVertices.size() - 2);

    transformedIndices.push_back(transformedVertices.size() - 4);
    transformedIndices.push_back(transformedVertices.size() - 2);
    transformedIndices.push_back(transformedVertices.size() - 1);
}

float resolve_equation(int index,float coord,float coordVect)
{
    float constante1 = 0, constante2 = 0;
    if (index == 0 || index > 3)
        constante1 = -4;
    else 
        constante1 = 4;
    if (index == 2 || index == 4)
        constante2 = 4;
    else 
        constante2 = -4;
    
    return (-(constante1*coordVect)+constante2)/constante1*coord;
}

void Rasterizer::ClipLines(const Entity& entity, FrameBuffer* pTarget, 
                           const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                           //outputs :
                           std::vector<Vertex>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices)
{
//    //indices.size() % 2 must be 0
//
    Vertex vertex1 = vertices[*it++];
    Vertex vertex2 = vertices[*it++];
    Vec4 vert1 = entity.transformation * Vec4(vertex1.position, 1);
    Vec4 vert2 = entity.transformation * Vec4(vertex2.position, 1);

    Vec4 norm1 = entity.transformation * Vec4(vertex1.normal, 1);
    Vec4 norm2 = entity.transformation * Vec4(vertex2.normal, 1);

    
    if (!vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {

    }
    if (!vert1.isInsideWSizedCube() && vert2.isInsideWSizedCube())
    {
        if (vert1.x > vert1.w || vert1.x < -vert1.w)
        {
            if (vert1.x > vert1.w)
                vert1.x = vert1.w;
            else 
                vert1.x = vert1.w;
        }
        else if (vert1.y > vert1.w || vert1.y < -vert1.w)
        {
            if (vert1.x > vert1.w)
                vert1.y = vert1.w;
            else 
                vert1.y = vert1.w;
        }
        else if (vert1.z > vert1.w || vert1.z < -vert1.w)
        {
            if (vert1.x > vert1.w)
                vert1.y = vert1.w;
            else 
                vert1.y = vert1.w;
        }
    }
    else if (vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {
        float dist1 = vert1.w - vert1.x;
        float dist2 = vert2.x - vert1.w;
        float total = dist2+dist1;
        Vec4 vert3 = vert1;
        Vec4 vert4 = vert2;
        Vec4 dir = {(vert2.x-vert1.x),(vert2.y-vert1.y),(vert2.z-vert1.z),1};
        Vec4 unit = {0,0,0,1};
        unit.x = dir.x/ sqrt(pow(dir.x, 2) + pow(dir.y,2));
        unit.y = dir.y/ sqrt(pow(dir.x, 2) + pow(dir.y,2));
        unit.z = dir.z/ sqrt(pow(dir.x, 2) + pow(dir.y,2));
        //unit.Homogenize();
        if (vert2.x > 1)
        {
            vert4.x = dir.x*(dist1/total);
            vert4.y = dir.y*(dist1/total);
            vert4.z = dir.z*(dist1/total);
        }
        if (vert2.x < -1)
        {
            vert4.x = -dir.x*(dist1/total);
            vert4.y = -dir.y*(dist1/total);
            vert4.z = -dir.z*(dist1/total);
        }
        std::cout << vert4.x <<std::endl;
        std::cout << vert4.y <<std::endl;
        addClippedVertex(vert3,{255,255,0,0}, norm1, transformedVertices,transformedIndices);
        addClippedVertex(vert4,{255,255,0,0}, norm2, transformedVertices,transformedIndices);

    }   
}

void Rasterizer::ClipTriangles(const Entity& entity, FrameBuffer* pTarget, 
                               const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                               //outputs :
                               std::vector<Vertex>& transformedVertices, 
                               std::vector<unsigned int>& transformedIndices)
{
    Vertex vertex1 = vertices[*it++];
    Vertex vertex2 = vertices[*it++];
    Vertex vertex3 = vertices[*it++];

    Vec4 vert1 = entity.transformation * Vec4(vertex1.position, 1);
    Vec4 vert2 = entity.transformation * Vec4(vertex2.position, 1);
    Vec4 vert3 = entity.transformation * Vec4(vertex3.position, 1);

    Vec4 norm1 = entity.transformation * Vec4(vertex1.normal, 0);
    Vec4 norm2 = entity.transformation * Vec4(vertex2.normal, 0);
    Vec4 norm3 = entity.transformation * Vec4(vertex3.normal, 0);

    // -w < x < w
    // for ()

    //if (isInsideScreen(vert1))
    {
        addClippedVertex(vert1, vertex1.color, norm1, transformedVertices, transformedIndices);
        addClippedVertex(vert2, vertex2.color, norm2, transformedVertices, transformedIndices);
        addClippedVertex(vert3, vertex3.color, norm3, transformedVertices, transformedIndices);
    }
}

void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->ResetPixels();
    
    std::vector<Vertex> transformedVertices;
    transformedVertices.reserve(100);
    std::vector<unsigned int> indices; 
    indices.reserve(100);

    std::function<void(FrameBuffer*, 
                       std::vector<Vertex>&, 
                       std::vector<unsigned int>::iterator&)> RenderByShape = RenderTriangles;

    std::function<void(const Entity&, 
                       FrameBuffer*, 
                       const std::vector<Vertex>&, 
                       std::vector<unsigned int>::iterator&,
                       std::vector<Vertex>& transformedVertices,
                       std::vector<unsigned int>&)> ClipShape = ClipTriangles;
 
    //render vertices of each entity
    for (const Entity& entity : pScene->entities)
    {
        std::vector<unsigned int>::iterator it = entity.mesh->indices.begin();
        while (it != entity.mesh->indices.end())
        {
            //Vertex2 vertex = convertVertexToWindow(entity.transformation, entity.mesh->vertices[*it++]);
            //Vertex2 vertex = convertVertexToWindow(entity.transformation, entity.mesh->vertices[*it++]);

            ClipShape(entity, pTarget, entity.mesh->vertices, it, transformedVertices, indices);
            
            //transformedVertices.push_back();


            // Vec4 newLoc = entity.transformation * Vec4(vertex.position, 1);

            // -w < x < w
        }
    }

    //render vertices of each entity
    std::vector<unsigned int>::iterator it = indices.begin();
    while (it != indices.end())
    {
        //RenderPoints(entity, pTarget, entity.mesh->vertices, it);
        RenderByShape(pTarget, transformedVertices, it);
        //RenderByShape(pTarget, transformedVertices, it);
    }
}
