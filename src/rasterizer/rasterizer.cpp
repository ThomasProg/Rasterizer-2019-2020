#include <cassert>
#include <cmath>
#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include <set>
#include <cassert>

#include "dynamicAsStaticArray.h"

#include "rasterizer.h"
#include "clipping.h"

#include "light.h"

#include "macros.h"

#include "scene.h"
#include "texture.h"
#include "frameBuffer.h"

#include "camera.h"
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

// typedef std::vector<unsigned int>::iterator indiceIt;

// Vec3 Rasterizer::projection(const Vec3& vec)
// {
//     return Vec3(((vec.x/5) + 1) * 0.5 * windowWidth, windowHeight - ((vec.y/5) + 1) * 0.5 * windowHeight, vec.z);

//     // return Vec3((vec.x/2.f + 0.5f) * windowWidth,
//     //             (vec.y/2.f + 0.5f) * windowHeight, vec.z);
// }

// /* 
//  * if used, draw vertices as points
//  * Complexity : O(1)
//  */
// // void Rasterizer::RenderPoints(FrameBuffer* pTarget, std::vector<Light>& lights, 
// //                               std::vector<Vertex>& vertices, indiceIt& it)
// // {
// //     RasterizingVertex vertex;
// //     vertex.setFromVertex(vertices[*it++], Rasterizer::projection);
// //     //Vertex2 screenVertex = convertVertexToWindow(entity.transformation, vertex);

// //     pTarget->SetPixel(vertex.position2D.x, vertex.position2D.y, vertex.position2D.z, vertex.color);
// // }

// void Rasterizer::RenderPoints(FrameBuffer* pTarget, 
//                             const std::vector<Light>& lights,
//                             const std::vector<Vec3>&  screenVertices, 
//                             const std::vector<Vertex>& worldVertices,
//                             Texture* texture)
// {
//     for (unsigned int i = 0; i < screenVertices.size(); i++)
//     {
//         pTarget->SetPixel(screenVertices[i].x, screenVertices[i].y, screenVertices[i].z, worldVertices[i].color);
//     }
// }

// void drawLine(FrameBuffer* pTarget, const Vertex& vertex1, const Vertex& vertex2)
// {
//     //get distance between two points
//     const int& x1 = vertex1.position.x;
//     const int& y1 = vertex1.position.y;
//     const int& x2 = vertex2.position.x;
//     const int& y2 = vertex2.position.y;
    
//     Vec2 vec2(x2-x1, y2-y1);
//     //get distance between the 2 points
//     float magnitude = vec2.GetMagnitude();

//     if (magnitude != 0.f)
//     {
//         vec2.toUnit();

//         for (float i = 0; i <= magnitude; ++i)
//         {
//             float ratio = i / magnitude;
//             Vec3 point(x1 + i * vec2.x, 
//                        y1 + i * vec2.y, 
//                        vertex1.position.z * ratio + vertex2.position.z * (1 - ratio));
//             //std::cout << "z : " << point.z << std::endl;

//             // float weight1 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
//             // float weight2 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
            
//             //Color c(vertex1.color.r, vertex1.color.g, vertex1.color.b);
//             Color c(255,255,255);

//             pTarget->SetPixel(point.x, point.y, point.z, c);
//         }
//     }
//     else 
//         pTarget->SetPixel(x2, y2, vertex2.position.z, vertex1.color);
// }

// /* 
//  * if used, draw vertices as lines
//  * Complexity : O(n), n being the distance on screen between the 2 points
//  */
// void Rasterizer::RenderLines(FrameBuffer* pTarget, std::vector<Light>& lights,
//                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
// {
//     //get first point on screen
//     const Vertex& vertex1 = vertices[*it++];
//     const Vertex& vertex2 = vertices[*it++];

//     drawLine(pTarget, vertex1, vertex2);
// }

// void Rasterizer::RenderWireframe(FrameBuffer* pTarget, 
//                                 const std::vector<Light>& lights,
//                                 const std::vector<Vec3>&  screenVertices, 
//                                 const std::vector<Vertex>& worldVertices,
//                                 Texture* texture)
// {
//     //wireframe mode
//     drawLine(pTarget, screenVertices[0], screenVertices[1]);
//     drawLine(pTarget, screenVertices[1], screenVertices[2]);
//     drawLine(pTarget, screenVertices[2], screenVertices[0]);
// }

inline
unsigned int getNbMixedColors(const int x, const int y, const FrameBuffer& highResolutionFB, const float curDepth)
{
    unsigned int nbMixedColors = 0;

    for (unsigned int i = 0; i < antiAliasingY; i++)
    {
        for (unsigned int j = 0; j < antiAliasingX; j++)
        {    
            unsigned int highResIndex = x * antiAliasingX + j + 
                                        (i + y * antiAliasingY) * highResolutionFB.texture.width;
            if (highResolutionFB.depthBuffer.depth[highResIndex] < depthMax 
                && highResolutionFB.depthBuffer.depth[highResIndex] <= curDepth)
            {
                nbMixedColors++;
            }
        }
    }

    return nbMixedColors;
}

void Rasterizer::antiAliasingCompression(const FrameBuffer& highResolutionFB, Texture& finalTexture)
{  
    const Texture& highResolutionTexture = highResolutionFB.texture;
    for (unsigned int y = 0; y < finalTexture.height; y++)
    {
        for (unsigned int x = 0; x < finalTexture.width; x++)
        {
            //unsigned int id = x * antiAliasingX + y * (target.width) * antiAliasingY;
            Color finalColor (0, 0, 0, 0);

            #ifdef __MULTI_SAMPLING__
            unsigned int firstElemID = x * antiAliasingX + 
                                    (y * antiAliasingY) * highResolutionFB.texture.width;

            float curDepth = highResolutionFB.depthBuffer.depth[firstElemID];
            
            unsigned int nbMixedColors = getNbMixedColors(x, y, highResolutionFB, curDepth);

            if (nbMixedColors == 0)
                continue;
            
            //get pixel color
            for (unsigned int i = 0; i < antiAliasingY; i++)
            {
                for (unsigned int j = 0; j < antiAliasingX; j++)
                {       
                    unsigned int highResIndex = x * antiAliasingX + j + 
                                               (i + y * antiAliasingY) * highResolutionTexture.width;
                    if (highResolutionFB.depthBuffer.depth[highResIndex] < depthMax
                        && highResolutionFB.depthBuffer.depth[highResIndex] <= curDepth)
                    {
                        finalColor += highResolutionTexture.pixels[highResIndex] / nbMixedColors;
                    }
                }  
            }
            #endif

            #ifdef __SUPER_SAMPLING__

            //get pixel color
            for (unsigned int i = 0; i < antiAliasingY; i++)
            {
                for (unsigned int j = 0; j < antiAliasingX; j++)
                {       
                    unsigned int highResIndex = x * antiAliasingX + j + 
                                                                (i + y * antiAliasingY) * highResolutionTexture.width;

                    if (highResolutionFB.depthBuffer.depth[highResIndex] < depthMax)
                    {
                        finalColor += highResolutionTexture.pixels[highResIndex] / (antiAliasingY * antiAliasingX);
                    }
                }  
            }

            #endif

            finalTexture.pixels[x + y * finalTexture.width] = std::move(finalColor);
        }
    }
}

// void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, Camera& camera, E_RasterizerMode mode)
// {
//     assert(pScene != nullptr && pTarget != nullptr);

//     //init render texture
//     pTarget->ResetPixels();

//     for (const Entity& entity : pScene->entities)
//     {
//         std::vector<Vec3> worldLoc;     //relative to Camera
//         std::vector<Vec3> worldNormals; //relative to Camera

//         worldLoc.reserve(entity.mesh->vertices.size());
//         worldNormals.reserve(entity.mesh->vertices.size());

//         for (const Vertex& vertex : entity.mesh->vertices)
//         {
//             worldLoc.emplace_back(entity.transformation * Vec4(vertex.position, 1));
//             worldNormals.emplace_back(entity.transformation * Vec4(vertex.normal, 0));
//         }

//         std::vector<Vec3> culledLoc;
//         std::vector<Vec3> culledNormals;
//         std::vector<Vec2> culledUVs;
//         //culledLoc.reserve(worldLoc.size());

//         for (unsigned int i = 0; i < entity.mesh->indices.size(); i+=3)
//         {
//             unsigned int id1 = entity.mesh->indices[i];
//             unsigned int id2 = entity.mesh->indices[i+1];
//             unsigned int id3 = entity.mesh->indices[i+2];

//             //Vec3 cameraLoc = worldLoc[id1] - inverseCameraMatrix * Vec4(1, 1, 1, 1);
//             Vec3 cameraLoc = camera.cartesianLocation;

//             //float cameraLength = camera.location.z;

//             Vec3 N = crossProduct(Vec3((worldLoc[id2] - worldLoc[id1])), Vec3((worldLoc[id3] - worldLoc[id1])));

//             #ifdef __BACKFACE_CULLING__

//             if (dotProduct(N, worldLoc[id1] - cameraLoc) > 0)
//             {
//                 continue;
//             }

//             #endif

//             culledLoc.emplace_back(worldLoc[id1]);
//             culledLoc.emplace_back(worldLoc[id2]);
//             culledLoc.emplace_back(worldLoc[id3]);

//             culledNormals.emplace_back(worldNormals[id1]);
//             culledUVs.emplace_back(entity.mesh->vertices[id1].u, entity.mesh->vertices[id1].v);

//             culledNormals.emplace_back(worldNormals[id2]);
//             culledUVs.emplace_back(entity.mesh->vertices[id2].u, entity.mesh->vertices[id2].v);
            
//             culledNormals.emplace_back(worldNormals[id3]);
//             culledUVs.emplace_back(entity.mesh->vertices[id3].u, entity.mesh->vertices[id3].v);
//         }

//         std::vector<Vec4> notRelativeToCameraLoc;
//         notRelativeToCameraLoc.reserve(culledLoc.size());
//         for (const Vec4& loc3D : culledLoc)
//         {
//             notRelativeToCameraLoc.emplace_back((inverseCameraMatrix * loc3D));
//         }

//         std::vector<Vec4> screenLoc;
//         screenLoc.reserve(notRelativeToCameraLoc.size());

//         // TODO : CLIPPING

//         std::vector<float> verticesW;
//         verticesW.reserve(notRelativeToCameraLoc.size());

//         for (const Vec4& loc3D : notRelativeToCameraLoc)
//         {
//             Vec4 projectedLoc = projectionMatrix * loc3D;
//             verticesW.emplace_back(projectedLoc.w);
//             screenLoc.emplace_back(projectedLoc.getHomogenizedVec());
//         }

//         std::vector<Vec4> scaledLoc;
//         scaledLoc.reserve(screenLoc.size());

//         for (const Vec4& loc3D : screenLoc)
//         {
//             scaledLoc.emplace_back(Mat4::CreateScreenConversionMatrix() * loc3D);
//         }

//         for (unsigned int i = 0; i < scaledLoc.size(); i += 3)
//         {
//             unsigned int id1 = i;
//             unsigned int id2 = i+1;
//             unsigned int id3 = i+2;

//             Vec3 v1 = scaledLoc[id1];
//             Vec3 v2 = scaledLoc[id2];
//             Vec3 v3 = scaledLoc[id3];

//             Vertex vert1 = v1;
//             vert1.normal = culledNormals[id1];
//             vert1.u = culledUVs[id1].x;
//             vert1.v = culledUVs[id1].y;
//             Vertex vert2 = v2;
//             vert2.normal = culledNormals[id2];
//             vert2.u = culledUVs[id2].x;
//             vert2.v = culledUVs[id2].y;
//             Vertex vert3 = v3;
//             vert3.normal = culledNormals[id3];
//             vert3.u = culledUVs[id3].x;
//             vert3.v = culledUVs[id3].y;

//             //triangle default color
//             __FORCE_UNTEXTURED_TRIANGLE_COLOR__

//             vert1.color.a *= entity.alpha;
//             vert2.color.a *= entity.alpha;
//             vert3.color.a *= entity.alpha;

//             if (mode == E_RasterizerMode::E_WIREFRAME)
//             {
//                 drawLine(pTarget, vert1, vert2);
//                 drawLine(pTarget, vert2, vert3);
//                 drawLine(pTarget, vert3, vert1);
//             }
//             else
//                 drawTriangle(vert1, vert2, vert3, culledLoc[id1], culledLoc[id2], culledLoc[id3], 
//                                 verticesW[id1], verticesW[id2], verticesW[id3],
//                                 camera.cartesianLocation,
//                                 pTarget, pScene->lights, entity.mesh->pTexture);
//         }
//     }
// }

#include "renderTriangle2.h"

inline
bool compareEntitiesToDisplay(const Entity* lhs, const Entity* rhs)
{
    return lhs->alpha < rhs->alpha;
}


void renderEntities(std::vector<const Entity*>& entities, std::vector<Light>& lights, 
                    FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, 
                    Camera& camera, E_RasterizerMode mode)
{
    const Mat4 screenConversionMatrix = Mat4::CreateScreenConversionMatrix();
    std::array<float, 3> w;

    RenderTriangle2 rendering;

    for (const Entity* entity : entities)
    {
        const std::vector<Vertex>& vertices = entity->mesh->worldVertices;
        const std::vector<unsigned int>& indices = entity->mesh->indices;

        //#pragma omp parallel for
        for (unsigned int indicesIndex = 0; indicesIndex < indices.size(); indicesIndex += 3)
        {
            rendering.setupForTriangle(vertices[indices[indicesIndex]], 
                                        vertices[indices[indicesIndex + 1]], 
                                        vertices[indices[indicesIndex + 2]]);

            // rendering.setLocalToWorld(entity->transformation);

            if (!rendering.isBackFaceCulled(camera.cartesianLocation))
            {
                rendering.setRelativeToCamera(inverseCameraMatrix);

                w = rendering.projectVertices(projectionMatrix);

                rendering.setVerticesToScreenResolution(screenConversionMatrix);

                //rendering.setDefaultColor();

                rendering.addTransparency(entity->alpha);
            
                // //rendering.drawWireframe(pTarget);


                // rendering.v1.color = Color(250,250,250);   
                // rendering.v2.color = Color(250,100,250);   
                // rendering.v3.color = Color(250,250,100);


                rendering.drawTriangleX(pTarget, w, camera.cartesianLocation, lights, entity->mesh->pTexture, entity->mat);    
            }
        }
    }
}
#include <algorithm>
void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget, 
                            const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, 
                            Camera& camera, E_RasterizerMode mode)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->ResetPixels();

    #ifdef __ENABLE_TRANSPARENCY__
    // std::set<const Entity*, bool(*)(const Entity* lhs, const Entity* rhs)> sortedEntities (&compareEntitiesToDisplay);

    std::vector<const Entity*> opaqueEntities;
    std::vector<const Entity*> transparentEntities;

    for (const Entity& entity : pScene->entities)
    {
        if (entity.mesh != nullptr)
        {
            entity.mesh->worldVertices.reserve(entity.mesh->vertices.size());
            for (const Vertex& vert : entity.mesh->vertices)
            {
                entity.mesh->worldVertices.emplace_back(vert);
                entity.mesh->worldVertices[entity.mesh->worldVertices.size() - 1].changeRef(entity.transformation);
            }
        }
    }

    for (Entity& entity : pScene->entities)
    {
        entity.computeCenter();
    }

    for (const Entity& entity : pScene->entities)
    {
        if (entity.alpha == 1)
            opaqueEntities.emplace_back(&entity);
        else 
            transparentEntities.emplace_back(&entity);
    }

    std::sort(transparentEntities.begin(), transparentEntities.end(), 
        [&camera] (const Entity* a, const Entity* b)
        {
            // return true if center of a is closer
            // sqrt is not used to calcul the distance because we don't need it to compare them
            const Vec3 dist1 = a->center - camera.cartesianLocation;
            const Vec3 dist2 = b->center - camera.cartesianLocation;

            float f1 = dist1.getLengthSquared(), f2 = dist2.getLengthSquared();
            return (a->center - camera.cartesianLocation).getLengthSquared() 
                    > (b->center - camera.cartesianLocation).getLengthSquared();
        }  
    );
    
    renderEntities(opaqueEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);

    renderEntities(transparentEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);
    #else
    //renderEntities(pScene->entities, pScene->lights, pTarget, projectionMatrix, inverseCameraMtrix, camera, mode);
    #endif
}
