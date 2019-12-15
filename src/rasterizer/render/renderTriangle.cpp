#include <cmath>
#include <algorithm>
#include <cassert>
#include "maths/utilityFunctions.h"

#include "renderTriangle.h"

#include "rasterizer.h"
#include "frameBuffer.h"
#include "vertex.h"
#include "rasterizingVertex.h"
#include "color.h"
#include "light.h"
#include "material.h"
#include "texture.h"

#include "macros.h"

template<class T>
__inline constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (v > hi) ? hi : v;
}

float RenderTriangle::getPixelLight(const Vec3& location3D, 
                                    const Vec3& normal, 
                                    const std::vector<Light>& lights, 
                                    const Vec3& cameraLocation, const Material& mat)
{
    float ambient  = 0.f;
    float diffuse  = 0.f;
    float specular = 0.f;  
    float total    = 0.f;

    for (const Light& light : lights)
    {
        //ambient
        ambient = light.ambientComponent * mat.ambient;

        //diffuse
        Vec3 pixelToLightVec = (light.position - location3D);
        pixelToLightVec.Normalize();
        
        float cosTeta = std::max(0.f, dotProduct(pixelToLightVec, normal));

        diffuse = light.diffuseComponent * mat.diffuse * cosTeta;

        //specular
        Vec3 pixelToEyeVec = cameraLocation - location3D;
        // pixelToEyeVec.z -= 1;
        pixelToEyeVec.Normalize();
        Vec3 h = pixelToLightVec + pixelToEyeVec;
        h.Normalize();
        float cosAlpha = dotProduct(normal.getNormalized(), h);

        if (cosAlpha < 0)
            cosAlpha = 0;

        specular = light.specularComponent * mat.specular * std::pow(cosAlpha, mat.brillance);

        total += ambient + diffuse + specular;
    }

    return total;
}

// ////////Get weight of a point in a triangle//////////
// inline
// bool getWeight(const Vec2& p, const Vec3& p1, const Vec3& p2, const Vec3& p3, std::array<float, 3>& weight)
// {
//     float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
//     if (det == 0)
//     {
//         weight[0] = 0;
//         weight[1] = 0;
//         weight[2] = 0;
//         return false;
//     }

//     weight[0] = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
//     weight[1] = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
//     weight[2] = 1 - weight[1] - weight[0];
    
//     return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
// }
 
// __inline
// bool tryToDrawPixel(unsigned int x, unsigned int y, bool& isInside,
//                     const Vec3& v1, const Vec3& v2, const Vec3& v3,
//                     std::array<Vertex*, 3>& triangleVertices, 
//                     std::array<float, 3>&  ww, float* uP, float* vP, const Vec3& cameraLocation,
//                     std::array<Vec3, 3>& worldLoc, FrameBuffer* pTarget, std::vector<Light>& lights, 
//                     Texture* texture, const Material& mat, bool bShouldRecur)
// {   
//     //BECAREFUL
//     if (!(x > 0 && x < pTarget->width && y > 0 && y < pTarget->height))
//         return false;

//     std::array<float, 3> weight;
//     bool isValid = getWeight(Vec2(x,y), v1, v2, v3, weight);

//     Vec3 location3D(0,0,0);

//     // could be unrolled
//     for (unsigned int i = 0; i < 3; i++)
//     {
//         location3D.x += weight[i] * worldLoc[i]->x;
//         location3D.y += weight[i] * worldLoc[i]->y;
//         location3D.z += weight[i] * worldLoc[i]->z;
//     }

//     //compute z
//     float z = (v1.z) * weight[0] + (v2.z) * weight[1] + (v3.z) * weight[2];

//     if (!isValid)
//         return false;

//     #ifdef __PERSPECTIVE_FIX__
//     // weight[0] *= ww[0];
//     // weight[1] *= ww[1];
//     // weight[2] *= ww[2];
//     weight[0] /= ww[0];
//     weight[1] /= ww[1];
//     weight[2] /= ww[2];
//     float total = weight[0] + weight[1] + weight[2];
//     if (total == 0)
//         return false;
        
//     weight[0] = weight[0] / total;
//     weight[1] = weight[1] / total;
//     weight[2] = weight[2] / total;
//     #endif

//     //BECAREFUL
//     float currentDepth = pTarget->depthBuffer.getDepth(x, y);

//     if (currentDepth <= z)
//         return false;

//     float intensity = 1.f;

//     //RasterizingVertex vert;
//     Vec3 normal(0, 0, 0);
//     Color c(0, 0, 0, 0);

//     float u = 0;
//     float v = 0;

//     // could be unrolled
//     for (unsigned int i = 0; i < 3; i++)
//     {
//         normal += weight[i] * triangleVertices[i]->normal;
//     }
    
//     normal.Normalize();

//     #ifdef __BLINN_PHONG_LIGHTING__
//     intensity = RenderTriangle::getPixelLight(location3D, normal, lights, cameraLocation, mat);
//     #endif


//     if (texture != nullptr && texture->pixels != nullptr)
//     {
//         // could be unrolled
//         for (unsigned int i = 0; i < 3; i++)
//         {
//             u += weight[i] * uP[i];
//             v += weight[i] * vP[i];
//         }

//         //if not in range, due to float imprecision
//         // u = clamp(u, 0.f, 1.f);
//         // v = clamp(v, 0.f, 1.f);
//         u = fmod(u, 1);
//         if (u < 0)
//             u += 1;
//         v = fmod(v, 1);
//         if (v < 0)
//             v += 1;
        
//         //TODO : enum for interpolation type

//         #ifdef __NEAREST_INTERPOLATION__
//         {
//             getTexturedColorNearestInterpolation(texture, 
//                                                  u, v, 
//                                                  c);
//         }
//         #endif

//         #ifdef __BILINEAR_INTERPOLATION__
//         {
//             getTexturedColorBilinearInterpolation(texture, 
//                                                   u, v, 
//                                                   c);
//         }
//         #endif

//         c.a += weight[0] * triangleVertices[0]->color.a
//              + weight[1] * triangleVertices[1]->color.a
//              + weight[2] * triangleVertices[2]->color.a;
//     }
//     #if defined(__NEAREST_INTERPOLATION__) || defined(__BILINEAR_INTERPOLATION__)
//     else 
//     #endif
//         getUntexturedPixelColor(triangleVertices, weight, c);

//     Vec3 newLoc(0,0,0);
//     // could be unrolled
//     for (unsigned int i = 0; i < 3; i++)
//     {
//         newLoc.x += weight[i] * worldLoc[i]->x;
//         newLoc.y += weight[i] * worldLoc[i]->y;
//         newLoc.z += weight[i] * worldLoc[i]->z;
//     }

//     mat.additionalShaders(c, newLoc);

//     //c *= intensity;
//     c.r *= intensity;
//     c.g *= intensity;
//     c.b *= intensity;

//     pTarget->SetPixel(x, y, z, c);

//     return true;
// }

// void drawTriangle(Vertex& vert1, Vertex& vert2, Vertex& vert3, Vec3 worldLoc1, Vec3 worldLoc2, Vec3 worldLoc3, 
//                     float w1, float w2, float w3,
//                     const Vec3& cameraLocation, FrameBuffer* pTarget, std::vector<Light>& lights, 
//                     const Material& mat, Texture* texture)
// {
//     std::array<Vertex*, 3> triangleVertices;
//     triangleVertices[0] = &vert1;
//     triangleVertices[1] = &vert2;
//     triangleVertices[2] = &vert3;

//     #ifdef __PERSPECTIVE_FIX__
//     // TODO: assert?
//     if (w1 == 0 || w2 == 0 || w3 == 0)
//         return;
//     // For a large amount of operations,
//     // multiplication is faster than divison on most processors,
//     // even though it is architecture dependant.
//     // http://ithare.com/infographics-operation-costs-in-cpu-clock-cycles/
//     float ww[3];
//     // ww[0] = 1 / w1;
//     // ww[1] = 1 / w2;
//     // ww[2] = 1 / w3;
//     ww[0] = w1;
//     ww[1] = w2;
//     ww[2] = w3;
//     #endif

//     // TODO : TODELETE : junk
//     bool isInside = false;

//     std::array<Vec3*, 3> worldLoc;
//     worldLoc[0] = &worldLoc1;
//     worldLoc[1] = &worldLoc2;
//     worldLoc[2] = &worldLoc3;

//     const Vec3& v1 = vert1.position;
//     const Vec3& v2 = vert2.position;
//     const Vec3& v3 = vert3.position;

//     //getWeight constants

//     //BECAREFUL
//     //clipping in rasterization
//     unsigned int minX = std::min(std::max(0.f, std::min(std::min(v1.x, v2.x), v3.x)), float(pTarget->width));
//     unsigned int maxX = std::min(std::max(0.f, std::max(std::max(v1.x, v2.x), v3.x)), float(pTarget->width));
//     unsigned int minY = std::min(std::max(0.f, std::min(std::min(v1.y, v2.y), v3.y)), float(pTarget->height));
//     unsigned int maxY = std::min(std::max(0.f, std::max(std::max(v1.y, v2.y), v3.y)), float(pTarget->height));

//     // BECAREFUL : not necessary, since loops already do the job
//     // if (maxX < minX || maxY < minY)
//     //     return;

//     //float weight[3];

//     // float intensityVertex[3];
//     // intensityVertex[0] = getPixelLight(*triangleVertices[0]);
//     // intensityVertex[1] = getPixelLight(*triangleVertices[1]);
//     // intensityVertex[2] = getPixelLight(*triangleVertices[2]);

//     float uP[3] = {vert1.u,vert2.u,vert3.u};
//     float vP[3] = {vert1.v,vert2.v,vert3.v};

//     #ifdef __MULTI_SAMPLING__
//     // every pixel respecting (x % 2 == 0 && y % 2 == 0)) 
//     // construct the base texture.
//     minX -= minX % 2;
//     minY -= minY % 2;
//     maxX -= maxX % 2;
//     maxY -= maxY % 2;
//     #endif

//     //TODO : set WeightVar outside of the loops
//     #ifdef __MULTI_SAMPLING__
//     for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
//     {
//         isInside = false;
//         for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
//         {
//     #else 
//     for (unsigned int y = minY; y <= maxY; y ++)
//     {
//         isInside = false;
//         for (unsigned int x = minX; x <= maxX; x ++)
//         {
//     #endif

//             tryToDrawPixel(x, y, isInside, 
//                             v1, v2, v3, 
//                             triangleVertices,
//                             ww, uP, vP, cameraLocation, 
//                             worldLoc, pTarget, lights, texture, mat);
//         }
//     }
    
//     #ifdef __MULTI_SAMPLING__

//     bool bOld = false;
//     for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
//     {
//         bOld = false;
//         for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
//         {
//             float weight[3];
//             bool bNew = getWeight(Vec2(x,y), v1, v2, v3, weight);
//             // if (bNew != bOld)
//             if (bNew && !bOld)
//             {
//                 for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
//                 {
//                     for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
//                     {
//                         const float currentX = x + (iAliasing - antiAliasingX);
//                         const float currentY = y + (jAliasing - antiAliasingY);
//                         if (!tryToDrawPixel(currentX, currentY, isInside, 
//                                         v1, v2, v3, 
//                                         triangleVertices,
//                                         ww, uP, vP, cameraLocation, 
//                                         worldLoc, pTarget, lights, texture, mat))
//                         {
//                             if (pTarget->GetDepth(currentX, currentY) >= depthMax)
//                             {
//                                 pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
//                                 #ifdef __ANTI_ALIASING_DEBUG__
//                                 pTarget->SetPixel(currentX, currentY, 0, Color(1, 0, 0, 1));
//                                 #endif
//                             }
//                         }
//                     }
//                 }
//             }
//             if (!bNew && bOld)
//             {
//                 for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
//                 {
//                     for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
//                     {
//                         const float currentX = x + (iAliasing - antiAliasingX);
//                         const float currentY = y + (jAliasing - antiAliasingY);
//                         if (!tryToDrawPixel(currentX, currentY, isInside, 
//                                         v1, v2, v3, 
//                                         triangleVertices,
//                                         ww, uP, vP, cameraLocation, 
//                                         worldLoc, pTarget, lights, texture, mat))
//                         {
//                             if (pTarget->GetDepth(currentX, currentY) >= depthMax)
//                             {
//                                 pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
//                                 #ifdef __ANTI_ALIASING_DEBUG__
//                                 pTarget->SetPixel(currentX, currentY, 0, Color(0, 1, 0, 1));
//                                 #endif
//                             }
//                         }
//                     }
//                 }
//             }
//             bOld = bNew;
//         }
//     }

//     for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
//     {
//         bOld = false;
//         for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
//         {
//             float weight[3];
//             bool bNew = getWeight(Vec2(x,y), v1, v2, v3, weight);
//             if (bNew && !bOld)
//             {
//                 for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
//                 {
//                     for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
//                     {
//                         const float currentX = x + (iAliasing - antiAliasingX);
//                         const float currentY = y + (jAliasing - antiAliasingY);
//                         if (!tryToDrawPixel(currentX, currentY, isInside, 
//                                         v1, v2, v3, 
//                                         triangleVertices,
//                                         ww, uP, vP, cameraLocation, 
//                                         worldLoc, pTarget, lights, texture, mat))
//                         {
//                             if (pTarget->GetDepth(currentX, currentY) >= depthMax)
//                             {
//                                 pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
//                                 #ifdef __ANTI_ALIASING_DEBUG__
//                                 pTarget->SetPixel(currentX, currentY, 0, Color(0, 0, 1, 1));
//                                 #endif
//                             }
//                         }
//                     }
//                 }
//             }
//             if (!bNew && bOld)
//             {
//                 for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
//                 {
//                     for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
//                     {
//                         const float currentX = x + (iAliasing - antiAliasingX);
//                         const float currentY = y + (jAliasing - antiAliasingY);
//                         // if (currentX > minX && currentY > minY && currentX < maxX && currentY < maxY)
//                         // {
//                             if (!tryToDrawPixel(currentX, currentY, isInside, 
//                                             v1, v2, v3, 
//                                             triangleVertices,
//                                             ww, uP, vP, cameraLocation, 
//                                             worldLoc, pTarget, lights, texture, mat))
//                             {
//                                 if (pTarget->GetDepth(currentX, currentY) >= depthMax)
//                                 {
//                                     pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
//                                     #ifdef __ANTI_ALIASING_DEBUG__
//                                     pTarget->SetPixel(currentX, currentY, 0, Color(1, 1, 0, 1));
//                                     #endif
//                                 }
//                             }
//                         //}
//                     }
//                 }
//             }
//             bOld = bNew;
//         }
//     }
//     #endif
// }
