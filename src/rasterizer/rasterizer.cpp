#include <cassert>
#include <cmath>
#include <iostream>
#include <functional>
#include <array>
#include <vector>
#include <set>
#include <cassert>
#include <algorithm>

#include "rasterizer.h"

#include "renderTriangle2.h"
#include "light.h"

#include "macros.h"

#include "scene.h"
#include "texture.h"
#include "frameBuffer.h"

#include "camera.h"
#include "entity.h"
#include "vertex.h"
#include "mesh.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "color.h"

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

            // #ifdef __MULTI_SAMPLING_TRIANGLES__
            // unsigned int firstElemID = x * antiAliasingX + 
            //                         (y * antiAliasingY) * highResolutionFB.texture.width;

            // float curDepth = highResolutionFB.depthBuffer.depth[firstElemID];
            
            // unsigned int nbMixedColors = getNbMixedColors(x, y, highResolutionFB, curDepth);

            // if (nbMixedColors == 0)
            //     continue;
            
            // //get pixel color
            // for (unsigned int i = 0; i < antiAliasingY; i++)
            // {
            //     for (unsigned int j = 0; j < antiAliasingX; j++)
            //     {       
            //         unsigned int highResIndex = x * antiAliasingX + j + 
            //                                    (i + y * antiAliasingY) * highResolutionTexture.width;
            //         if (highResolutionFB.depthBuffer.depth[highResIndex] < depthMax
            //             && highResolutionFB.depthBuffer.depth[highResIndex] <= curDepth)
            //         {
            //             finalColor += highResolutionTexture.pixels[highResIndex] / nbMixedColors;
            //         }
            //     }  
            // }
            // #endif

            // #ifdef __SUPER_SAMPLING__
            #if defined(__SUPER_SAMPLING__) || defined(__MULTI_SAMPLING_LIGHT__)

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

void renderEntities(std::vector<const Entity*>& entities, std::vector<Light>& lights, 
                    FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, 
                    Camera& camera, E_RasterizerMode mode)
{
    // We get the transform matrices.
    const Mat4 screenConversionMatrix = Mat4::CreateScreenConversionMatrix();
    const Mat4 cameraAndProjectMatrix = projectionMatrix * inverseCameraMatrix;


    RenderTriangle2 rendering;

    // For each entity
    for (const Entity* entity : entities)
    {
        // Get aliases for vertices and indices
        const std::vector<Vertex>& vertices = entity->mesh->worldVertices;
        const std::vector<unsigned int>& indices = entity->mesh->indices;

        // For each triangle (each 3 indices), render it
        // Note : Some data is being computed more than 1 time because of identic indices.
        //        This function could be optimized by first :
        //          - computed data for each vertex
        //          - storing them into a buffer array
        //          - use this data to render them on screen
        //        However, it would mean the backface culling would happen next, 
        //        because the backface culling is linked to the triangle,
        //        and performance could be on the contrary lost.
        //        We could also use the back face culled with precomputed normals,
        //        but it would be not necessarily the true normal and thus be imprecise.
        for (unsigned int indicesIndex = 0; indicesIndex < indices.size(); indicesIndex += 3)
        {
            rendering.setupForTriangle(vertices[indices[indicesIndex]], 
                                       vertices[indices[indicesIndex + 1]], 
                                       vertices[indices[indicesIndex + 2]]);

            // rendering.setLocalToWorld(entity->transformation);
            #ifdef __BACKFACE_CULLING__
            if (!rendering.isBackFaceCulled(camera.cartesianLocation))
            #endif
            { 
                // Contains projected vertices in Vec4, necessary to clip them.
                std::array<Vec4, 3> projectedVertices;
                std::array<float, 3> w;

                for (unsigned int i = 0; i < projectedVertices.size(); i++)
                {
                    projectedVertices[i] = cameraAndProjectMatrix * rendering.triangleVertices[i].position;
                    w[i] = projectedVertices[i].w;
                }

                // Contains triangles added by clipping.
                std::vector<RenderTriangle2> additionnalTriangles;

                if (rendering.isClipped(pTarget->texture, additionnalTriangles, projectedVertices) 
                    && additionnalTriangles.size() == 0)
                    continue;

                // Render each triangle

                rendering.triangleVertices[0].position = projectedVertices[0].getHomogenizedVec();
                rendering.triangleVertices[1].position = projectedVertices[1].getHomogenizedVec();
                rendering.triangleVertices[2].position = projectedVertices[2].getHomogenizedVec();

                rendering.projectAndDraw(lights, entity, pTarget, 
                                         projectionMatrix, screenConversionMatrix,
                                         camera, mode, w);
                                         
                for (RenderTriangle2& triangle : additionnalTriangles)
                {
                    triangle.triangleVertices[0].position = projectedVertices[0].getHomogenizedVec();
                    triangle.triangleVertices[1].position = projectedVertices[1].getHomogenizedVec();
                    triangle.triangleVertices[2].position = projectedVertices[2].getHomogenizedVec();

                    triangle.projectAndDraw(lights, entity, pTarget, 
                                            projectionMatrix, screenConversionMatrix,
                                            camera, mode, w);
                }
            }
        }
    }
}

void Rasterizer::RenderScene(Scene* pScene, FrameBuffer* pTarget, 
                            const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, 
                            Camera& camera, E_RasterizerMode mode)
{
    // If the scene or the framebuffer is invalid, this function should not be called.
    // It is a logic error.
    assert(pScene != nullptr && pTarget != nullptr);

    // init render texture
    pTarget->ResetPixels();

    // Get worldLocation for each vertex of each entity.
    // If an entity has no vertices in its default array :
    //     - if it has vertices in worldVertices, it will use them (performance gain)
    //     - else, nothing is done
    for (const Entity& entity : pScene->entities)
    {
        if (entity.mesh != nullptr && entity.mesh->vertices.size() != 0)
        {
            entity.mesh->worldVertices.clear();
            entity.mesh->worldVertices.reserve(entity.mesh->vertices.size());
            for (const Vertex& vert : entity.mesh->vertices)
            {
                entity.mesh->worldVertices.emplace_back(vert);
                entity.mesh->worldVertices[entity.mesh->worldVertices.size() - 1].changeRef(entity.transformation);
            }
        }
    }

    #ifdef __ENABLE_TRANSPARENCY__

    // If transparency is enabled, opaque entities should be put first into the buffer.
    // The reason is to be sure opaque entities will be rendered behind transparent ones.

    std::vector<const Entity*> opaqueEntities;
    std::vector<const Entity*> transparentEntities;

    // We compute the entity "center" to get an idea of its global location.
    for (Entity& entity : pScene->entities)
    {
        entity.computeCenter();
    }

    for (const Entity& entity : pScene->entities)
    {
        if (entity.mesh != nullptr)
        {
            if (entity.alpha >= 1)
                opaqueEntities.emplace_back(&entity);
            else 
                transparentEntities.emplace_back(&entity);
        }
    }

    // For transparent entities, we have to sort them by distance to player order.
    // It is to not have bad calculs and to render both of them.
    std::sort(transparentEntities.begin(), transparentEntities.end(), 
        [&camera] (const Entity* a, const Entity* b)
        {
            // return true if center of a is closer
            return (a->center - camera.cartesianLocation).getLengthSquared() 
                    > (b->center - camera.cartesianLocation).getLengthSquared();
        }  
    );

    renderEntities(opaqueEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);

    renderEntities(transparentEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);
    #else
    // Even if we don't have transparent entities, 
    // we have to put them into an array to call the function without changing its signature
    std::vector<const Entity*> opaqueEntities;
    opaqueEntities.reserve(pScene->entities.size());
    for (const Entity& entity : pScene->entities)
    {
        if (entity.mesh != nullptr)
            opaqueEntities.emplace_back(&entity);
    }   
    renderEntities(opaqueEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);
    #endif
}
