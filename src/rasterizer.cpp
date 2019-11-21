#include <cassert>
#include <iostream>
#include <functional>

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

Vec2 projection(Vec3 vec)
{
    return Vec2((vec.x/2.f + 0.5f) * windowWidth,
                (vec.y/2.f + 0.5f) * windowHeight);
}

// Vertex2 convertVertexToWindow(const Mat4& transformation, const Vertex& vertex)
// {
//     Vertex2 windowVertex2;
//     windowVertex2.color = vertex.color;

//     Vec4 newLoc = transformation * Vec4(vertex.position, 1);

//     -w < x < w

//     newLoc.Homogenize();

//     Vec3 newLoc3D(newLoc.x, newLoc.y, newLoc.z);
//     windowVertex2.position = projection(newLoc3D);

//     return windowVertex2;
// }

/* 
 * if used, draw vertices as points
 * Complexity : O(1)
 */
void Rasterizer::RenderPoints(Texture* pTarget, 
                              const std::vector<Vertex2>& vertices, indiceIt& it)
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
                            const std::vector<Vertex2>& vertices, std::vector<unsigned int>::iterator& it)
{
    //get first point on screen
    const Vertex2& vertex1 = vertices[*it++];
    const Vertex2& vertex2 = vertices[*it++];

    //convertVertexToWindow(entity.transformation, vertex1.position);
    //Vertex2 screenVertex1 = convertVertexToWindow(entity.transformation, vertex1);

    //get second point on screen
    //const Vertex& vertex2 = entity.mesh->vertices[*it++];
    //Vertex2 screenVertex2 = convertVertexToWindow(entity.transformation, vertex2);

    //Color c(0,0,255,0);

    //get distance between two points
    const int& x1 = vertex1.position.x;
    const int& y1 = vertex1.position.y;
    const int& x2 = vertex2.position.x;
    const int& y2 = vertex2.position.y;
    Vec2 vec2(x2-x1, y2-y1);
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
        return false;

    weight[0] = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
    weight[1] = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
    weight[2] = 1 - weight[1] - weight[0];
    
    return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
}


// /////////////////////////////////////////////////
// ///////////////////START///////////////////////
// /////////////////////////////////////////////////


// Vec2 A, B, C; //TODO : TODELETE : DELETE

// ///////draw an horizontal line//////// O(n)
// //TODO : add Vertex
// void drawHorizontalLine(int x1, int x2, int height, Texture* pTarget)
// {
//     Color c(255, 0, 0, 255);
//     float rgb[3];
//     for (int x = x2; x < x1; x++)
//     {
//         getWeight(Vec2(x, height), A, B, C, rgb);
//         c.r = rgb[0]*255;
//         c.g = rgb[1]*255;
//         c.b = rgb[2]*255;
        
//         pTarget->SetPixelColor(x, height, c);
//     }
// }

// //TODO : replace Vec2 by Vertex
// /* 
//  * if used, draw vertices as BottomFlatTriangle
//  * Complexity : O(n^2)
//  */
// void fillBottomFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Texture* pTarget)
// {
//     A = v1;
//     B = v2;
//     C = v3;
//     float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
//     float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

//     float curx1 = v1.x;
//     float curx2 = v1.x;

//     for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
//     {
//         drawHorizontalLine((int)curx1, (int)curx2, scanlineY, pTarget);
//         curx1 += invslope1;
//         curx2 += invslope2;
//     }
// }

// //TODO : replace Vec2 by Vertex
// /* 
//  * if used, draw vertices as BottomFlatTriangle
//  * Complexity : O(n^2)
//  */
// void fillTopFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Texture* pTarget)
// {
//     float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
//     float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

//     float curx1 = v3.x;
//     float curx2 = v3.x;

//     for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
//     {
//         drawHorizontalLine((int)curx1, (int)curx2, scanlineY, pTarget);
//         curx1 -= invslope1;
//         curx2 -= invslope2;
//     }
// }

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

// void sortPointsByY(Vertex2*& array)
// {

// }

// void Rasterizer::RenderTriangles(const Entity& entity, Texture* pTarget, 
//                             const std::vector<Vertex2>& vertices, std::vector<unsigned int>::iterator& it)
// {
//     Vertex* triangleVertices[3];

//     triangleVertices[0] = &entity.mesh->vertices[*it++];
//     int x1, y1;
//     Vertex2 p1;
//     convertVertexToWindow(entity.transformation, triangleVertices[0]->position, p1, x1, y1);

//     triangleVertices[1] = &entity.mesh->vertices[*it++];
//     int x2, y2;
//     convertVertexToWindow(entity.transformation, triangleVertices[1]->position, x2, y2);

//     triangleVertices[2] = &entity.mesh->vertices[*it++];
//     int x3, y3;
//     convertVertexToWindow(entity.transformation, triangleVertices[2]->position, x3, y3);


//     //fillBottomFlatTriangle(Vec2(x1, y1), Vec2(x2, y2), Vec2(x3, y3), pTarget);

//    /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
//   //sortVerticesAscendingByY();
//   //if (v1.y > v2.y && v1.y > )

//   /* here we know that v1.y <= v2.y <= v3.y */
//   /* check for trivial case of bottom-flat triangle */
//   if (v2.y == v3.y)
//   {
//     fillBottomFlatTriangle(v1, v2, v3, pTarget);
//   }
//   /* check for trivial case of top-flat triangle */
//   else if (v1.y == v2.y)
//   {
//     fillTopFlatTriangle(v1, v2, v3, pTarget);
//   }
//   else
//   {
//     /* general case - split the triangle in a topflat and bottom-flat one */
//     Vec2 v4 = Vec2(
//       (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
//     fillBottomFlatTriangle(v1, v2, v4, pTarget);
//     fillTopFlatTriangle(v2, v4, v3, pTarget);
//   }
// }

// /////////////////////////////////////////////////
// ///////////////////END///////////////////////
// /////////////////////////////////////////////////

void addClippedVertex(const Vec4& vert,
                      Color color,
                      std::vector<Vertex2>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    transformedVertices.push_back(Vertex2(projection(vert.getHomogenizedVec()), color));
    transformedIndices.push_back(transformedVertices.size() - 1);
}

//vertices size should be 4
void add4ClippedVertex(const Vec4* vertices,
                      const Color* color,
                      std::vector<Vertex2>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    Vec3 homogenizedVec3 = vertices[0].getHomogenizedVec();

    for (unsigned int i = 0; i < 4; i++)
    {
        transformedVertices.push_back(Vertex2(projection(vertices[i].getHomogenizedVec()), color[i]));
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
                           std::vector<Vertex2>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices)
{
    
    Vertex vertex1 = vertices[*it++];
    Vertex vertex2 = vertices[*it++];
    Vec4 vert1 = entity.transformation * Vec4(vertex1.position, 1);
    Vec4 vert2 = entity.transformation * Vec4(vertex2.position, 1);

    if (vert1.isInsideWSizedCube() && vert2.isInsideWSizedCube())
    {
        addClippedVertex(vert1, vertex1.color, transformedVertices, transformedIndices);
        addClippedVertex(vert2, vertex2.color, transformedVertices, transformedIndices);
    }
    
    else if (!vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {

    }
    else if (!vert1.isInsideWSizedCube() && vert2.isInsideWSizedCube())
    {

    }
    else if (vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {

    }
    

}

void Rasterizer::ClipTriangles(const Entity& entity, Texture* pTarget, 
                               const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                               //outputs :
                               std::vector<Vertex2>& transformedVertices, 
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
    }
}

void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->FillBlack();

    std::vector<Vertex2> transformedVertices;
    std::vector<unsigned int> indices; 

    std::function<void(Texture*, 
                       const std::vector<Vertex2>&, 
                       std::vector<unsigned int>::iterator&)> RenderByShape = RenderLines;

    std::function<void(const Entity&, 
                       Texture*, 
                       const std::vector<Vertex>&, 
                       std::vector<unsigned int>::iterator&,
                       std::vector<Vertex2>& transformedVertices,
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
        RenderLines(pTarget, transformedVertices, it);
        //RenderByShape(pTarget, transformedVertices, it);
    }
    //TODO : 
    //Sutherrland Hardgeman algorithm
    //optimization
}
