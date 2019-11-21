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
    return Vec3((vec.x/2.f + 0.5f) * windowWidth,
                (vec.y/2.f + 0.5f) * windowHeight, vec.z);
}

/* 
 * if used, draw vertices as points
 * Complexity : O(1)
 */
void Rasterizer::RenderPoints(Texture* pTarget, 
                              const std::vector<Vertex>& vertices, indiceIt& it)
{
    const Vertex2& vertex = vertices[*it++];

    //Vertex2 screenVertex = convertVertexToWindow(entity.transformation, vertex);

    pTarget->SetPixelColor(vertex.position.x, vertex.position.y, vertex.color);
}

/* 
 * if used, draw vertices as lines
 * Complexity : O(n), n being the distance on screen between the 2 points
 */
void Rasterizer::RenderLines(Texture* pTarget, 
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
            Color c(255,0,0);
            pTarget->SetPixelColor(point.x, point.y, c);
        }
    }
    else 
        pTarget->SetPixelColor(x2, y2, vertex1.color);
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
// ///////////////////START///////////////////////
// /////////////////////////////////////////////////

Vertex M_VERTICES[3]; //TODO : TODELETE : DELETE

Color getColorInTriangle(Vec2 p, const std::array<const Vertex*, 3>& triangleVertices)
{
    Color c(0,0,0);
    float weight[3];
    getWeight(p, triangleVertices[0]->position, triangleVertices[1]->position, triangleVertices[2]->position, weight);

    for (unsigned int i = 0; i < 3; i++)
    {
        c.r += weight[0] * triangleVertices[i]->color.r;
        c.g += weight[1] * triangleVertices[i]->color.g;
        c.b += weight[2] * triangleVertices[i]->color.b;
    }

        //std::cout << (int)triangleVertices[0]->color.g << '\n';

    return c;
}

// ///////draw an horizontal line//////// O(n)
// //TODO : add Vertex
//
void drawHorizontalLine(Vec2 xMinMax, int height, Texture* pTarget, const std::array<const Vertex*, 3>& triangleVertices2)
{
    Color c(0, 0, 0, 255);
    float weight[3];
std::array<const Vertex*, 3> triangleVertices;
triangleVertices[0] = &M_VERTICES[0];
triangleVertices[1] = &M_VERTICES[1];
triangleVertices[2] = &M_VERTICES[2];

    if (xMinMax.x > xMinMax.y)
        for (int x = xMinMax.y; x < xMinMax.x; x++)
        {
            // c = Color(0,0,0);
            // getWeight(Vec2(x, height), 
            //             triangleVertices[0]->position, 
            //             triangleVertices[1]->position, 
            //             triangleVertices[2]->position, 
            //             weight);

            // for (unsigned int i = 0; i < 3; i++)
            // {
            //     c.r += weight[0] * triangleVertices[i]->color.r;
            //     c.g += weight[1] * triangleVertices[i]->color.g;
            //     c.b += weight[2] * triangleVertices[i]->color.b;
            // }

            c = getColorInTriangle(Vec2(x, height), triangleVertices);

            // getWeight(Vec2(x, height), 
            //             triangleVertices[0]->position, 
            //             triangleVertices[1]->position, 
            //             triangleVertices[2]->position, 
            //             weight);

            // for (unsigned int i = 0; i < 3; i++)
            // {
            //     c.r += weight[0] * M_VERTICES[i].color.r;
            //     c.g += weight[1] * M_VERTICES[i].color.g;
            //     c.b += weight[2] * M_VERTICES[i].color.b;
            // }

            // c.r = weight[0] * M_VERTICES[0].color.r;
            // c.g = weight[1] * M_VERTICES[1].color.g;
            // c.b = weight[2] * M_VERTICES[2].color.b;

            // c.r = weight[0] * 255;
            // c.g = weight[1] * 255;
            // c.b = weight[2] * 255;
            
            pTarget->SetPixelColor(x, height, c);
        }
    else 
        for (int x = xMinMax.x; x < xMinMax.y; x++)
        {
            // c = Color(0,0,0);
            // getWeight(Vec2(x, height), 
            //             triangleVertices[0]->position, 
            //             triangleVertices[1]->position, 
            //             triangleVertices[2]->position, 
            //             weight);

            // for (unsigned int i = 0; i < 3; i++)
            // {
            //     c.r += weight[0] * triangleVertices[i]->color.r;
            //     c.g += weight[1] * triangleVertices[i]->color.g;
            //     c.b += weight[2] * triangleVertices[i]->color.b;
            // }
            c = getColorInTriangle(Vec2(x, height), triangleVertices);

            // getWeight(Vec2(x, height), 
            //             triangleVertices[0]->position, 
            //             triangleVertices[1]->position, 
            //             triangleVertices[2]->position, 
            //             weight);

            // for (unsigned int i = 0; i < 3; i++)
            // {
            //     c.r += weight[0] * M_VERTICES[i].color.r;
            //     c.g += weight[1] * M_VERTICES[i].color.g;
            //     c.b += weight[2] * M_VERTICES[i].color.b;
            // }
            // c.r = weight[0] * M_VERTICES[0].color.r;
            // c.g = weight[1] * M_VERTICES[1].color.g;
            // c.b = weight[2] * M_VERTICES[2].color.b;

            // c.r = weight[0] * 255;
            // c.g = weight[1] * 255;
            // c.b = weight[2] * 255;

            pTarget->SetPixelColor(x, height, c);
        }
    //std::cout << (int)triangleVertices[0]->color.r << "\n";
}

// //TODO : replace Vec2 by Vertex
// /* 
//  * if used, draw vertices as BottomFlatTriangle
//  * Complexity : O(n^2)
//  */
void fillBottomFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices, Texture* pTarget)
{
    // A = v1;
    // B = v2;
    // C = v3;
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    Vec2 xMinMax (v1.x, v1.x);

    for (int verticalLoc = v1.y; verticalLoc <= v2.y; verticalLoc++)
    {
        drawHorizontalLine(xMinMax, verticalLoc, pTarget, triangleVertices);
        xMinMax.x += invslope1;
        xMinMax.y += invslope2;
    }
}

//TODO : replace Vec2 by Vertex
/* 
 * if used, draw vertices as BottomFlatTriangle
 * Complexity : O(n^2)
 */
void fillTopFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices, Texture* pTarget)
{
    // A = v1;
    // B = v2;
    // C = v3;
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    Vec2 xMinMax(v3.x, v3.x);

    for (int verticalLoc = v3.y; verticalLoc > v1.y; verticalLoc--)
    {
        drawHorizontalLine(xMinMax, verticalLoc, pTarget, triangleVertices);
        xMinMax.x -= invslope1;
        xMinMax.y -= invslope2;
    }
}

// // /* 
// //  * if used, draw vertices as BottomFlatTriangle
// //  * Complexity : Best case  : O(n^2)
// //  *              Worst case : O(2 * n^2)
// //  */
// // void drawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Texture* pTarget)
// // {
// //    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
// //   //sortVerticesAscendingByY();
// //   //if (v1.y > v2.y && v1.y > )

// //   /* here we know that v1.y <= v2.y <= v3.y */
// //   /* check for trivial case of bottom-flat triangle */
// //   if (v2.y == v3.y)
// //   {
// //     fillBottomFlatTriangle(v1, v2, v3, pTarget);
// //   }
// //   /* check for trivial case of top-flat triangle */
// //   else if (v1.y == v2.y)
// //   {
// //     fillTopFlatTriangle(v1, v2, v3, pTarget);
// //   }
// //   else
// //   {
// //     /* general case - split the triangle in a topflat and bottom-flat one */
// //     Vec2 v4 = Vec2(
// //       (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
// //     fillBottomFlatTriangle(v1, v2, v4, pTarget);
// //     fillTopFlatTriangle(v2, v4, v3, pTarget);
// //   }
// // }

void Rasterizer::RenderTriangles(Texture* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    std::array<const Vertex*, 3> triangleVertices;

    //get first point on screen
    triangleVertices[0] = &vertices[*it++];
    triangleVertices[1] = &vertices[*it++];
    triangleVertices[2] = &vertices[*it++];

    M_VERTICES[0] = *triangleVertices[0];
    M_VERTICES[1] = *triangleVertices[1];
    M_VERTICES[2] = *triangleVertices[2];

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
    Vec3 homogenizedVec3 = vertices[0].getHomogenizedVec();

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

void Rasterizer::ClipLines(const Entity& entity, Texture* pTarget, 
                           const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                           //outputs :
                           std::vector<Vertex>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices)
{
    //indices.size() % 2 must be 0

    Vertex vertex1 = vertices[*it++];
    Vertex vertex2 = vertices[*it++];
    Vec4 vert1 = entity.transformation * Vec4(vertex1.position, 1);
    Vec4 vert2 = entity.transformation * Vec4(vertex2.position, 1);

    float coeff_dir = (vert2.y-vert1.x)/(vert2.y-vert1.x);
    float OrdonneOrigin = vert1.y+coeff_dir*vert1.x;
    float norme = sqrtf(pow((vert2.x-vert1.x),2) + pow((vert2.y-vert1.y),2));
    
    if (!vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {
        
    }
    else if (!vert1.isInsideWSizedCube() && vert2.isInsideWSizedCube())
    {
        float x = vert1.y;
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

    }
    else if (vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {
        if (vert2.x > vert2.w || vert2.x < -vert2.w)
        {
            if (vert2.x > vert2.w)
                vert2.x = vert2.w;
            else 
                vert2.x = -vert2.w;
        }
        else if (vert2.y > vert2.w || vert2.y < -vert2.w)
        {
            if (vert2.x > vert2.w)
                vert2.y = vert2.w;
            else 
                vert2.y = -vert2.w;
        }
    }
    addClippedVertex(vert1, vertex1.color, transformedVertices, transformedIndices);
    addClippedVertex(vert2, vertex2.color, transformedVertices, transformedIndices);
    

}

void Rasterizer::ClipTriangles(const Entity& entity, Texture* pTarget, 
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

void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->FillBlack();

    std::vector<Vertex> transformedVertices;
    std::vector<unsigned int> indices; 

    std::function<void(Texture*, 
                       const std::vector<Vertex>&, 
                       std::vector<unsigned int>::iterator&)> RenderByShape = RenderTriangles;

    std::function<void(const Entity&, 
                       Texture*, 
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
