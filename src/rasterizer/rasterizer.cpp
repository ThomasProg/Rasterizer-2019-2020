#include <cassert>
#include <cmath>
#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include <cassert>

#include "dynamicAsStaticArray.h"

#include "rasterizer.h"
#include "clipping.h"

#include "light.h"

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
// void Rasterizer::RenderPoints(FrameBuffer* pTarget, std::vector<Light>& lights, 
//                               std::vector<Vertex>& vertices, indiceIt& it)
// {
//     RasterizingVertex vertex;
//     vertex.setFromVertex(vertices[*it++], Rasterizer::projection);
//     //Vertex2 screenVertex = convertVertexToWindow(entity.transformation, vertex);

//     pTarget->SetPixel(vertex.position2D.x, vertex.position2D.y, vertex.position2D.z, vertex.color);
// }

void Rasterizer::RenderPoints(FrameBuffer* pTarget, 
                            const std::vector<Light>& lights,
                            const std::vector<Vec3>&  screenVertices, 
                            const std::vector<Vertex>& worldVertices,
                            Texture* texture)
{
    for (unsigned int i = 0; i < screenVertices.size(); i++)
    {
        pTarget->SetPixel(screenVertices[i].x, screenVertices[i].y, screenVertices[i].z, worldVertices[i].color);
    }
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
void Rasterizer::RenderLines(FrameBuffer* pTarget, std::vector<Light>& lights,
                            std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    //get first point on screen
    const Vertex& vertex1 = vertices[*it++];
    const Vertex& vertex2 = vertices[*it++];

    drawLine(pTarget, vertex1, vertex2);
}

// void Rasterizer::RenderWireframe(FrameBuffer* pTarget, std::vector<Light>& lights,
//                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
// {
//     std::array<RasterizingVertex, 3> triangleVertices;

//     //get first point on screen
//     triangleVertices[0].setFromVertex(vertices[*it++], Rasterizer::projection);
//     triangleVertices[1].setFromVertex(vertices[*it++], Rasterizer::projection);
//     triangleVertices[2].setFromVertex(vertices[*it++], Rasterizer::projection);

//     //wireframe mode
//     drawLine(pTarget, triangleVertices[0].position2D, triangleVertices[1].position2D);
//     drawLine(pTarget, triangleVertices[1].position2D, triangleVertices[2].position2D);
//     drawLine(pTarget, triangleVertices[2].position2D, triangleVertices[0].position2D);
// }

void Rasterizer::RenderWireframe(FrameBuffer* pTarget, 
                                const std::vector<Light>& lights,
                                const std::vector<Vec3>&  screenVertices, 
                                const std::vector<Vertex>& worldVertices,
                                Texture* texture)
{
    //wireframe mode
    drawLine(pTarget, screenVertices[0], screenVertices[1]);
    drawLine(pTarget, screenVertices[1], screenVertices[2]);
    drawLine(pTarget, screenVertices[2], screenVertices[0]);
}

void Rasterizer::RenderTriangles(FrameBuffer* pTarget, 
                                const std::vector<Light>& lights,
                                const std::vector<Vec3>&  screenVertices, 
                                const std::vector<Vertex>& worldVertices,
                                Texture* texture)
{
    // std::array<Vertex*, 3> triangleVertices3D;
    // std::array<Vertex, 3> triangleVertices2D;

    std::array<RasterizingVertex, 3> triangleVertices;

    //get first point on screen
    // triangleVertices[0].setFromVertex(vertices[*it++], Rasterizer::projection);
    // triangleVertices[1].setFromVertex(vertices[*it++], Rasterizer::projection);
    // triangleVertices[2].setFromVertex(vertices[*it++], Rasterizer::projection);

    for (unsigned int i = 0; i < 3; ++i)
    {
        triangleVertices[i].color = worldVertices[i].color;
        triangleVertices[i].normal = worldVertices[i].normal;
        triangleVertices[i].position3D = worldVertices[i].position;
        triangleVertices[i].position2D = screenVertices[i];
    }

    RenderTriangle tri(triangleVertices, pTarget, lights, texture);
    tri.draw();
}

// void Rasterizer::RenderTriangles(FrameBuffer* pTarget, std::vector<Light>& lights,
//                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
// {
//     // std::array<Vertex*, 3> triangleVertices3D;
//     // std::array<Vertex, 3> triangleVertices2D;

//     std::array<RasterizingVertex, 3> triangleVertices;
//     // triangleVertices[1].setFromVertex(vertices[1], projection);
//     // triangleVertices[2].setFromVertex(vertices[2], projection);

//     //get first point on screen
//     triangleVertices[0].setFromVertex(vertices[*it++], Rasterizer::projection);
//     triangleVertices[1].setFromVertex(vertices[*it++], Rasterizer::projection);
//     triangleVertices[2].setFromVertex(vertices[*it++], Rasterizer::projection);


//     // //draw normals
//     // Vec3 n1 = Rasterizer::projection(triangleVertices[0].normal*(1) + triangleVertices[0].position3D);
//     // Vec3 n2 = Rasterizer::projection(triangleVertices[1].normal*(1) + triangleVertices[1].position3D);
//     // Vec3 n3 = Rasterizer::projection(triangleVertices[2].normal*(1) + triangleVertices[2].position3D);
    
//     // drawLine(pTarget, triangleVertices[0].position2D, n1);
//     // drawLine(pTarget, triangleVertices[1].position2D, n2);
//     // drawLine(pTarget, triangleVertices[2].position2D, n3);

//     // //wireframe mode
//     // drawLine(pTarget, triangleVertices[0].position2D, triangleVertices[1].position2D);
//     // drawLine(pTarget, triangleVertices[1].position2D, triangleVertices[2].position2D);
//     // drawLine(pTarget, triangleVertices[2].position2D, triangleVertices[0].position2D);

//     //sort
//     // if (triangleVertices[0]->position.y > triangleVertices[1]->position.y)
//     //     std::swap(triangleVertices[0], triangleVertices[1]);
//     // if (triangleVertices[1]->position.y > triangleVertices[2]->position.y)
//     //     std::swap(triangleVertices[1], triangleVertices[2]);
//     // if (triangleVertices[0]->position.y > triangleVertices[1]->position.y)
//     //     std::swap(triangleVertices[0], triangleVertices[1]);

//     RenderTriangle tri(triangleVertices, pTarget, lights);
//     tri.draw();
// }

// /////////////////////////////////////////////////
// ///////////////////END///////////////////////
// /////////////////////////////////////////////////

// void addClippedVertex(const Vec4& vert,
//                       Color color,
//                       std::vector<Vertex>& transformedVertices, 
//                       std::vector<unsigned int>& transformedIndices)



// void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, E_RasterizerMode mode)
// {
//     assert(pScene != nullptr && pTarget != nullptr);

//     //init render texture
//     pTarget->ResetPixels();

//     std::function<bool(const DynamicAsStaticArray<Vec3>& screenVertices, const DynamicAsStaticArray<Vertex>& worldVertices, 
//                        std::vector<Vec3>& screenVerticesClipped, std::vector<Vertex>& worldVerticesClipped)> ClipShape;

//     std::function<void(FrameBuffer*, 
//                        const std::vector<Light>& lights,
//                        std::vector<Vec3>&  screenVertices, 
//                        std::vector<Vertex> worldVertices, Texture* texture)> RenderByShape;

//     //3 for a triangle, 2 for a line, 1 for a point
//     unsigned int nbVerticesPerRasterization;

//     std::vector<Vertex> worldVerticesClipped; // Contains points color, world location, and world normals.
//     std::vector<Vec3>   screenVerticesClipped; // Contains projected points location 2D. //TODO : Vec3 to Vec2

//     worldVerticesClipped.reserve(6); // Contains points color, world location, and world normals.
//     screenVerticesClipped.reserve(6); // Contains projected points location 2D. //TODO : Vec3 to Vec2

//     switch(mode)
//     {
//         case E_RasterizerMode::E_POINTS :
//             RenderByShape = RenderPoints;
//             ClipShape     = Clipping::ClipPoints;
//             nbVerticesPerRasterization = 1;
//             break;

//         case E_RasterizerMode::E_TRIANGLES :
//             RenderByShape = RenderTriangles;
//             ClipShape     = Clipping::ClipTriangles;
//             nbVerticesPerRasterization = 3;
//             break;

//         case E_RasterizerMode::E_WIREFRAME :
//         case E_RasterizerMode::E_TRIANGLES_AS_LINES :
//             RenderByShape = RenderWireframe;
//             ClipShape     = Clipping::ClipTriangles;
//             nbVerticesPerRasterization = 3;
//             break;
//     }

//     unsigned int maxVertices = 0;
//     for (const Entity& entity : pScene->entities)
//     {
//         if (maxVertices < entity.mesh->vertices.size())
//             maxVertices = entity.mesh->vertices.size();
//     }

//     DynamicAsStaticArray<Vertex> worldVertices(maxVertices);
//     DynamicAsStaticArray<Vec3> screenVertices(maxVertices);
//     DynamicAsStaticArray<bool> isCalculated(maxVertices);

//     for (const Entity& entity : pScene->entities)
//     {
//         for (unsigned int i = 0; i < entity.mesh->vertices.size(); i++)
//             isCalculated[i] = false;

//         DynamicAsStaticArray<Vertex> worldVerticesLocal(nbVerticesPerRasterization);
//         DynamicAsStaticArray<Vec3> screenVerticesLocal(nbVerticesPerRasterization);

//         for (unsigned int indice = 0; indice < entity.mesh->indices.size(); indice += nbVerticesPerRasterization)
//         {
//             for (unsigned int localIndice = 0; localIndice < nbVerticesPerRasterization; ++localIndice)
//             {
//                 int verticesIndice = entity.mesh->indices[indice + localIndice];

//                 if (!isCalculated[verticesIndice])
//                 {
//                     //transform localToWorld;
//                     Vertex worldVertex;
//                     worldVertex.position = (entity.transformation * 
//                                             Vec4(entity.mesh->vertices[verticesIndice].position, 1)).getHomogenizedVec();

//                     worldVertex.normal = (entity.transformation * 
//                                             Vec4(entity.mesh->vertices[verticesIndice].normal, 0));

//                     worldVertex.color = entity.mesh->vertices[verticesIndice].color;

//                     worldVertices[verticesIndice] = std::move(worldVertex);

//                     //project 3D world to 2D screen
//                     //Vec3 screenVec = projection(worldVertices[verticesIndice].position);
//                     Vec3 vTemp3 = worldVertices[verticesIndice].position;
//                     //vTemp3.z = 10;
//                     Vec4 screenVec = projectionMatrix * Vec4(vTemp3, 1);
//                     //std::cout << "A : " << screenVec.x << std::endl;
//                     screenVec.Homogenize();
//                     //std::cout << "B : " << screenVec.x << std::endl;
//                     screenVec = (Mat4::CreateScreenConversionMatrix() * Vec4(screenVec, 1)).getHomogenizedVec();

//                     screenVertices[verticesIndice] = std::move(screenVec);

//                     isCalculated[verticesIndice] = true;
//                 }

//                 worldVerticesLocal[localIndice]  = worldVertices[verticesIndice];
//                 screenVerticesLocal[localIndice] = screenVertices[verticesIndice];
//             }

//             screenVerticesClipped.clear();
//             worldVerticesClipped.clear();

//             //inputs  : array of projected vertices (2D)
//             //outputs : array of cut vertices (2D) 
//             bool isClipped = ClipShape(screenVerticesLocal, worldVerticesLocal, 
//                                     screenVerticesClipped, worldVerticesClipped);

//             //inputs : texture to draw in / lights for lighting / vertices (2D) to draw / vertices (3D) for lighting
//             // if (isClipped)
//             // {
//         //std::cout << "YER : " << worldVerticesClipped[0].position.x << std::endl;
//                 RenderByShape(pTarget, pScene->lights, screenVerticesClipped, worldVerticesClipped, entity.mesh->pTexture);
//             // }
//             // else  
//             // {
//             //     RenderByShape(pTarget, pScene->lights, screenVertices, worldVertices);
//             // }
//         }
//     }
// }

void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, E_RasterizerMode mode)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->ResetPixels();

    for (const Entity& entity : pScene->entities)
    {
        std::vector<Vec4> worldLoc;
        std::vector<Vec4> worldNormals;

        for (const Vertex& vertex : entity.mesh->vertices)
        {
            worldLoc.emplace_back(entity.transformation * Vec4(vertex.position, 1));
            worldNormals.emplace_back(entity.transformation * Vec4(vertex.normal, 0));
        }

        std::vector<Vec4> screenLoc;

        for (const Vec4& loc3D : worldLoc)
        {
            screenLoc.emplace_back(projectionMatrix * loc3D);
        }

        std::vector<Vec4> scaledLoc;

        for (const Vec4& loc3D : screenLoc)
        {
            scaledLoc.emplace_back(Mat4::CreateScreenConversionMatrix() * loc3D);
            if ((Mat4::CreateScreenConversionMatrix() * loc3D).w == 0)
                std::cout << "0" << std::endl;
        }


        for (unsigned int i = 0; i < entity.mesh->indices.size(); i += 3)
        {
            unsigned int id1 = entity.mesh->indices[i];
            unsigned int id2 = entity.mesh->indices[i+1];
            unsigned int id3 = entity.mesh->indices[i+2];

            Vec3 v1 = scaledLoc[id1].getHomogenizedVec();
            Vec3 v2 = scaledLoc[id2].getHomogenizedVec();
            Vec3 v3 = scaledLoc[id3].getHomogenizedVec();

            Vertex vert1 = v1;
            Vertex vert2 = v2;
            Vertex vert3 = v3;

            vert1.color = Color(250,250,250);
            vert2.color = Color(250,250,250);
            vert3.color = Color(250,250,250);

            drawLine(pTarget, v1, v2);
            drawLine(pTarget, v2, v3);
            drawLine(pTarget, v3, v1);
        }

        //RenderTriangles(pTarget, pScene->lights, , entity.mesh->pTexture);
    }
}
