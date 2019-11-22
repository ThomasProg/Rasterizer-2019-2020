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
#include "mesh.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "color.h"

typedef std::vector<unsigned int>::iterator indiceIt;

Vec3 projection(Vec3 vec)
{
    return Vec3(((vec.x/5) + 1) * 0.5 * windowWidth, windowHeight - ((vec.y/5) + 1) * 0.5 * windowHeight, 0);

    // return Vec3((vec.x/2.f + 0.5f) * windowWidth,
    //             (vec.y/2.f + 0.5f) * windowHeight, vec.z);
}

/* 
 * if used, draw vertices as points
 * Complexity : O(1)
 */
void Rasterizer::RenderPoints(FrameBuffer* pTarget, 
                              const std::vector<Vertex>& vertices, indiceIt& it)
{
    const Vertex& vertex = vertices[*it++];

    //Vertex2 screenVertex = convertVertexToWindow(entity.transformation, vertex);

    pTarget->SetPixel(vertex.position.x, vertex.position.y, vertex.position.z, vertex.color);
}

/* 
 * if used, draw vertices as lines
 * Complexity : O(n), n being the distance on screen between the 2 points
 */
void Rasterizer::RenderLines(FrameBuffer* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    //get first point on screen
    const Vertex& vertex1 = vertices[*it++];
    const Vertex& vertex2 = vertices[*it++];

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
            Vec3 point(x1 + i * vec2.x, y1 + i * vec2.y, 0);
            float weight1 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
            float weight2 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
            Color c(255,255,0);
            pTarget->SetPixel(point.x, point.y, point.z, c);
        }
    }
    else 
        pTarget->SetPixel(x2, y2, vertex2.position.z, vertex1.color);
}

////////Get weight of a point in a triangle//////////
bool getWeight(const Vec2& p, const Vec2& p1, const Vec2& p2, const Vec2& p3, float* weight)
{
    float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    if (det == 0)
    {
        weight[0] = 0;
        weight[1] = 0;
        weight[2] = 0;
        return false;
    }

    weight[0] = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
    weight[1] = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
    weight[2] = 1 - weight[1] - weight[0];
    
    return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
}

// /////////////////////////////////////////////////
// ///////////////////TRIANGLES///////////////////////
// /////////////////////////////////////////////////

Color getColorInTriangle(Vec2 p, const std::array<const Vertex*, 3>& triangleVertices)
{
    Color c(0,0,0);
    float weight[3];
    getWeight(p, triangleVertices[0]->position, triangleVertices[1]->position, triangleVertices[2]->position, weight);

    for (unsigned int i = 0; i < 3; i++)
    {
        c.r += weight[i] * triangleVertices[i]->color.r;
        c.g += weight[i] * triangleVertices[i]->color.g;
        c.b += weight[i] * triangleVertices[i]->color.b;
    }

    return c;
}

///////draw an horizontal line//////// O(n)
void drawHorizontalLine(Vec2 p1, Vec2 p2, int height, FrameBuffer* pTarget, const std::array<const Vertex*, 3>& triangleVertices)
{
    Color c;

    if (p1.x > p2.x)
        for (int x = p2.x+1; x < p1.x; x++)
        {
            c = getColorInTriangle(Vec2(x, height), triangleVertices);
            
            float depth;
            pTarget->SetPixel(x, height, p1.y, c);
        }
    else 
        for (int x = p1.x+1; x < p2.x; x++)
        {
            c = getColorInTriangle(Vec2(x, height), triangleVertices);

            float depth;
            pTarget->SetPixel(x, height, p1.y, c);
        }
}

// //TODO : replace Vec2 by Vertex
// /* 
//  * if used, draw vertices as BottomFlatTriangle
//  * Complexity : O(n^2)
//  */
void fillBottomFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices, FrameBuffer* pTarget)
{
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float invslopeZ1 = (v2.z - v1.z) / (v2.y - v1.y);
    float invslopeZ2 = (v3.z - v1.z) / (v3.y - v1.y);

    //Vec2 xMinMax (v1.x, v1.x);

    Vec2 p1(v1.x, v1.z), p2(v1.x, v1.z);

    for (float verticalLoc = v1.y; verticalLoc <= v2.y; verticalLoc++)
    {
        drawHorizontalLine(p1, p2, verticalLoc, pTarget, triangleVertices);
        p1.x += invslope1;
        p2.x += invslope2;

        p1.y += invslopeZ1;
        p2.y += invslopeZ2;
    }
}

//TODO : replace Vec2 by Vertex
/* 
 * if used, draw vertices as BottomFlatTr    // A = v1;
    // B = v2;
    // C = v3;iangle
 * Complexity : O(n^2)
 */
void fillTopFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices, FrameBuffer* pTarget)
{
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float invslopeZ1 = (v2.z - v1.z) / (v3.y - v1.y);
    float invslopeZ2 = (v3.z - v2.z) / (v3.y - v2.y);

    Vec2 p1(v3.x, v3.z), p2(v3.x, v3.z);

    for (float verticalLoc = v3.y; verticalLoc > v1.y; verticalLoc--)
    {
        drawHorizontalLine(p1, p2, verticalLoc, pTarget, triangleVertices);
        p1.x -= invslope1;
        p2.x -= invslope2;

        p1.y -= invslopeZ1;
        p2.y -= invslopeZ2;
    }
}

void Rasterizer::RenderTriangles(FrameBuffer* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    std::array<const Vertex*, 3> triangleVertices;

    //get first point on screen
    triangleVertices[0] = &vertices[*it++];
    triangleVertices[1] = &vertices[*it++];
    triangleVertices[2] = &vertices[*it++];

    //sort
    if (triangleVertices[0]->position.y > triangleVertices[1]->position.y)
        std::swap(triangleVertices[0], triangleVertices[1]);
    if (triangleVertices[1]->position.y > triangleVertices[2]->position.y)
        std::swap(triangleVertices[1], triangleVertices[2]);
    if (triangleVertices[0]->position.y > triangleVertices[1]->position.y)
        std::swap(triangleVertices[0], triangleVertices[1]);

    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
    //sortVerticesAscendingByY();
    //if (v1.y > v2.y && v1.y > )

    /* here we know that v1.y <= v2.y <= v3.y */
    /* check for trivial case of bottom-flat triangle */
    if (v2.y == v3.y)
    {
        fillBottomFlatTriangle(triangleVertices, pTarget);
    }
    /* check for trivial case of top-flat triangle */
    else if (v1.y == v2.y)
    {
        fillTopFlatTriangle(triangleVertices, pTarget);
    }
    else
    {
        /* general case - split the triangle in a topflat and bottom-flat one */
        Vertex vertex4 (Vec3(
            (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y, 1));

        //vertex4.color = getColorInTriangle(vertex4.position, triangleVertices);

        std::array<const Vertex*, 3> bottomTriangleVertices;
        bottomTriangleVertices[0] = triangleVertices[0];
        bottomTriangleVertices[1] = triangleVertices[1];
        bottomTriangleVertices[2] = &vertex4;

        std::array<const Vertex*, 3> topTriangleVertices;
        topTriangleVertices[0] = triangleVertices[1];
        topTriangleVertices[1] = &vertex4;
        topTriangleVertices[2] = triangleVertices[2];
        fillBottomFlatTriangle(bottomTriangleVertices, pTarget);
        fillTopFlatTriangle(topTriangleVertices, pTarget);
    }
}

// /////////////////////////////////////////////////
// ///////////////////END///////////////////////
// /////////////////////////////////////////////////

void addClippedVertex(const Vec4& vert,
                      Color color,
                      std::vector<Vertex>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    transformedVertices.push_back(Vertex(projection(vert.getHomogenizedVec()), color));
    transformedIndices.push_back(transformedVertices.size() - 1);
}

//vertices size should be 4
void add4ClippedVertex(const Vec4* vertices,
                      const Color* color,
                      std::vector<Vertex>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    for (unsigned int i = 0; i < 4; i++)
    {
        transformedVertices.push_back(Vertex(projection(vertices[i].getHomogenizedVec()), color[i]));
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
        Vec3 dir = {(vert2.x-vert1.x),(vert2.y-vert1.y),(vert2.z-vert1.z)};
        for (unsigned i = 0; i < 6; i++)
        {
            float t = resolve_equation(i,vert1[i/2],dir[i/2]);
            vert2.x = vert1.x+dir.x*t;
            vert2.y = vert1.y+dir.y*t;
            vert2.z = vert1.z+dir.z*t;
        }
    }   
    addClippedVertex(vert1, vertex1.color, transformedVertices, transformedIndices);
    addClippedVertex(vert2, vertex2.color, transformedVertices, transformedIndices);
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

    // -w < x < w
    // for ()

    //if (isInsideScreen(vert1))
    {
        addClippedVertex(vert1, vertex1.color, transformedVertices, transformedIndices);
        addClippedVertex(vert2, vertex2.color, transformedVertices, transformedIndices);
        addClippedVertex(vert3, vertex3.color, transformedVertices, transformedIndices);
    }
}

void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->ResetPixels();

    std::vector<Vertex> transformedVertices;
    std::vector<unsigned int> indices; 
    std::vector<char> depthBuffer;
    depthBuffer.reserve(pTarget->width * pTarget->height);

    std::function<void(FrameBuffer*, 
                       const std::vector<Vertex>&, 
                       std::vector<unsigned int>::iterator&)> RenderByShape = RenderLines;

    std::function<void(const Entity&, 
                       FrameBuffer*, 
                       const std::vector<Vertex>&, 
                       std::vector<unsigned int>::iterator&,
                       std::vector<Vertex>& transformedVertices,
                       std::vector<unsigned int>&)> ClipShape = ClipLines;

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
