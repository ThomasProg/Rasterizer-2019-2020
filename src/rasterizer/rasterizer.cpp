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

#include "renderTriangle2.h"

void renderEntities(std::vector<const Entity*>& entities, std::vector<Light>& lights, 
                    FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix, 
                    Camera& camera, E_RasterizerMode mode)
{
    const Mat4 screenConversionMatrix = Mat4::CreateScreenConversionMatrix();
    RenderTriangle2 rendering;
    std::array<float, 3> w;

    for (const Entity* entity : entities)
    {
        const std::vector<Vertex>& vertices = entity->mesh->worldVertices;
        const std::vector<unsigned int>& indices = entity->mesh->indices;

        #pragma omp parallel for private(rendering), private(w)
        for (unsigned int indicesIndex = 0; indicesIndex < indices.size(); indicesIndex += 3)
        {
            rendering.setupForTriangle(vertices[indices[indicesIndex]], 
                                        vertices[indices[indicesIndex + 1]], 
                                        vertices[indices[indicesIndex + 2]]);

            // rendering.setLocalToWorld(entity->transformation);

            if (!rendering.isBackFaceCulled(camera.cartesianLocation))
            {
                rendering.setRelativeToCamera(inverseCameraMatrix);

                if (rendering.isClipped(&pTarget->texture))
                    continue;

                w = rendering.projectVertices(projectionMatrix);

                rendering.setVerticesToScreenResolution(screenConversionMatrix);

                //rendering.setDefaultColor();

                rendering.addTransparency(entity->alpha);
            
                switch (mode)
                {
                    case E_RasterizerMode::E_WIREFRAME:
                    case E_RasterizerMode::E_TRIANGLES_AS_LINES:
                        rendering.drawWireframe(pTarget);
                        break;


                    // rendering.v1.color = Color(250,250,250);   
                    // rendering.v2.color = Color(250,100,250);   
                    // rendering.v3.color = Color(250,250,100);

                    case E_RasterizerMode::E_TRIANGLES:
                        rendering.drawTriangleX(pTarget, w, camera.cartesianLocation, lights, entity->mesh->pTexture, entity->mat);   
                        break; 

                    default:
                        break;
                }
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

    for (const Entity& entity : pScene->entities)
    {
        if (entity.mesh != nullptr)
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
    // std::set<const Entity*, bool(*)(const Entity* lhs, const Entity* rhs)> sortedEntities (&compareEntitiesToDisplay);

    std::vector<const Entity*> opaqueEntities;
    std::vector<const Entity*> transparentEntities;

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
            // const Vec3 dist1 = a->center - camera.cartesianLocation;
            // const Vec3 dist2 = b->center - camera.cartesianLocation;

            // float f1 = dist1.getLengthSquared(), f2 = dist2.getLengthSquared();
            return (a->center - camera.cartesianLocation).getLengthSquared() 
                    > (b->center - camera.cartesianLocation).getLengthSquared();
        }  
    );
    
    renderEntities(opaqueEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);

    renderEntities(transparentEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);
    #else
    std::vector<const Entity*> opaqueEntities;
    opaqueEntities.reserve(pScene->entities.size());
    for (const Entity& entity : pScene->entities)
    {
        opaqueEntities.emplace_back(&entity);
    }   
    renderEntities(opaqueEntities, pScene->lights, pTarget, projectionMatrix, inverseCameraMatrix, camera, mode);
    #endif
}
