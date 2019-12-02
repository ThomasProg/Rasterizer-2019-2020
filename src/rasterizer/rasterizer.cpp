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

void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, E_RasterizerMode mode)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->ResetPixels();

    for (const Entity& entity : pScene->entities)
    {
        std::vector<Vec4> worldLoc;
        std::vector<Vec4> worldNormals;

        worldLoc.reserve(entity.mesh->vertices.size());
        worldNormals.reserve(entity.mesh->vertices.size());

        for (const Vertex& vertex : entity.mesh->vertices)
        {
            worldLoc.emplace_back(entity.transformation * Vec4(vertex.position, 1));
            worldNormals.emplace_back(entity.transformation * Vec4(vertex.normal, 0));
        }

        std::vector<Vec4> screenLoc;
        screenLoc.reserve(worldLoc.size());

        for (const Vec4& loc3D : worldLoc)
        {
        //     Vec4 loc3D1 = worldLoc[i];
        //     Vec4 loc3D2 = worldLoc[i+1];
        //     Vec4 loc3D3 = worldLoc[i+2];

        //     Vec3 n = crossProduct(loc3D2 - loc3D1, 
        //                           loc3D3 - loc3D1);
        //     if (dotProduct(Vec3(0,0,0), n) < 0)
        //     {
        //         screenLoc.emplace_back((projectionMatrix * inverseCameraMatrix * loc3D1).getHomogenizedVec());
        //         screenLoc.emplace_back((projectionMatrix * inverseCameraMatrix * loc3D2).getHomogenizedVec());
        //         screenLoc.emplace_back((projectionMatrix * inverseCameraMatrix * loc3D3).getHomogenizedVec());
                 screenLoc.emplace_back((projectionMatrix * inverseCameraMatrix * loc3D).getHomogenizedVec());
        //     }
        }

        std::vector<Vec4> culledLoc;
        std::vector<Vec3> culledNormals;
        std::vector<Vec2> culledUVs;
        //culledLoc.reserve(worldLoc.size());

        for (unsigned int i = 0; i < entity.mesh->indices.size(); i+=3)
        {
            unsigned int id1 = entity.mesh->indices[i];
            unsigned int id2 = entity.mesh->indices[i+1];
            unsigned int id3 = entity.mesh->indices[i+2];

            // Vec3 cameraLoc = Vec3(0, 0, 0);//Vec3(windowWidth/2, windowHeight/2, 0);
            // Vec3 L = cameraLoc;
            // L = Vec3(0, 0, 1);
            // Vec3 N = crossProduct(Vec3((screenLoc[id2] - screenLoc[id1])), Vec3((screenLoc[id3] - screenLoc[id1])));
            //std::cout << N << std::endl;
            //if (dotProduct(L, N) >= 0)
            //drawLine(pTarget, Vertex(worldLoc[id1]), Vertex(Vec3(worldLoc[id1]) + N));
            // if (dotProduct(N, Vec3(0,0,-1)) < 0)
            // {
            //     std::cout << dotProduct(N, Vec3(0,0,-1)) << std::endl;
            //     continue;
            // }

            culledLoc.emplace_back(screenLoc[id1]);
            culledLoc.emplace_back(screenLoc[id2]);
            culledLoc.emplace_back(screenLoc[id3]);

            culledNormals.emplace_back(worldNormals[id1]);
            culledUVs.emplace_back(entity.mesh->vertices[id1].u, entity.mesh->vertices[id1].v);

            culledNormals.emplace_back(worldNormals[id2]);
            culledUVs.emplace_back(entity.mesh->vertices[id2].u, entity.mesh->vertices[id2].v);
            
            culledNormals.emplace_back(worldNormals[id3]);
            culledUVs.emplace_back(entity.mesh->vertices[id3].u, entity.mesh->vertices[id3].v);
        }

        std::vector<Vec4> scaledLoc;
        scaledLoc.reserve(culledLoc.size());

        for (const Vec4& loc3D : culledLoc)
        {
            scaledLoc.emplace_back(Mat4::CreateScreenConversionMatrix() * loc3D);
            //scaledLoc.emplace_back(loc3D*100 + Vec4(windowWidth/2, windowHeight/2, 0, 0));
        }

        //for (unsigned int i = 0; i < entity.mesh->indices.size(); i += 3)
        for (unsigned int i = 0; i < culledNormals.size(); i += 3)
        {
            // unsigned int id1 = entity.mesh->indices[i];
            // unsigned int id2 = entity.mesh->indices[i+1];
            // unsigned int id3 = entity.mesh->indices[i+2];

            // Vec3 v1 = scaledLoc[id1];//.getHomogenizedVec();
            // Vec3 v2 = scaledLoc[id2];//.getHomogenizedVec();
            // Vec3 v3 = scaledLoc[id3];//.getHomogenizedVec();

            unsigned int id1 = i;
            unsigned int id2 = i+1;
            unsigned int id3 = i+2;

            Vec3 v1 = scaledLoc[id1];//.getHomogenizedVec();
            Vec3 v2 = scaledLoc[id2];//.getHomogenizedVec();
            Vec3 v3 = scaledLoc[id3];//.getHomogenizedVec();

            // Vertex vert1 = v1;
            // vert1.normal = worldNormals[id1];
            // vert1.u = entity.mesh->vertices[id1].u;
            // vert1.v = entity.mesh->vertices[id1].v;
            // Vertex vert2 = v2;
            // vert2.normal = worldNormals[id2];
            // vert2.u = entity.mesh->vertices[id2].u;
            // vert2.v = entity.mesh->vertices[id2].v;
            // Vertex vert3 = v3;
            // vert3.normal = worldNormals[id3];
            // vert3.u = entity.mesh->vertices[id3].u;
            // vert3.v = entity.mesh->vertices[id3].v;

            Vertex vert1 = v1;
            vert1.normal = culledNormals[id1];
            vert1.u = culledUVs[id1].x;
            vert1.v = culledUVs[id1].y;
            Vertex vert2 = v2;
            vert2.normal = culledNormals[id2];
            vert2.u = culledUVs[id2].x;
            vert2.v = culledUVs[id2].y;
            Vertex vert3 = v3;
            vert3.normal = culledNormals[id3];
            vert3.u = culledUVs[id3].x;
            vert3.v = culledUVs[id3].y;

            vert1.color = Color(250,250,250);
            vert2.color = Color(250,100,250);
            vert3.color = Color(250,250,100);
            // drawLine(pTarget, v1, v2);
            // drawLine(pTarget, v2, v3);
            // drawLine(pTarget, v3, v1);

            vert1.color.a *= entity.alpha;
            vert2.color.a *= entity.alpha;
            vert3.color.a *= entity.alpha;
            
            drawTriangle(vert1, vert2, vert3, pTarget, pScene->lights, entity.mesh->pTexture);
        }

        //RenderTriangles(pTarget, pScene->lights, , entity.mesh->pTexture);
    }
}

