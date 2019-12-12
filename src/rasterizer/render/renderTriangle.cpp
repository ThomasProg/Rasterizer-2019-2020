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
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

float RenderTriangle::getPixelLight(const RasterizingVertex& vertex, const std::vector<Light>& lights, const Vec3& cameraLocation)
{
    Material mat;

    float ambient  = 0.f;
    float diffuse  = 0.f;
    float specular = 0.f;  
    float total    = 0.f;

    for (const Light& light : lights)
    {
        //ambient
        ambient = light.ambientComponent * mat.ambient;

        //diffuse
        Vec3 pixelToLightVec = (light.position - vertex.position3D);
        pixelToLightVec.Normalize();
        
        float cosTeta = std::max(0.f, dotProduct(pixelToLightVec, vertex.normal));

        diffuse = light.diffuseComponent * mat.diffuse * cosTeta;

        //specular
        Vec3 pixelToEyeVec = cameraLocation - vertex.position3D; //TODO
        // pixelToEyeVec.z -= 1;
        pixelToEyeVec.Normalize();
        Vec3 h = pixelToLightVec + pixelToEyeVec;
        h.Normalize();
        float cosAlpha = dotProduct(vertex.normal, h);

        if (cosAlpha < 0)
            cosAlpha = 0;

        specular = light.specularComponent * mat.specular * std::pow(cosAlpha, mat.brillance);

        total += ambient + diffuse + specular;
    }

    return total;
}

////////Get weight of a point in a triangle//////////
inline
bool getWeight(const Vec2& p, const Vec3& p1, const Vec3& p2, const Vec3& p3, float* weight)
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
 
bool tryToDrawPixel(unsigned int x, unsigned int y, bool& isValid, bool& isInside,
                    const Vec3& v1, const Vec3& v2, const Vec3& v3,
                    std::array<Vertex*, 3>& triangleVertices, 
                    float*  ww, float* uP, float* vP, const Vec3& cameraLocation,
                    std::array<Vec3*, 3>& worldLoc, FrameBuffer* pTarget, std::vector<Light>& lights, Texture* texture, bool bShouldRecur = true)
{   
    float weight[3];
    isValid = getWeight(Vec2(x,y), v1, v2, v3, weight);

    if (!isValid)
        return false;

    #ifdef __PERSPECTIVE_FIX__
    weight[0] /= ww[0];
    weight[1] /= ww[1];
    weight[2] /= ww[2];
    float total = weight[0] + weight[1] + weight[2];
    weight[0] = weight[0] / total;
    weight[1] = weight[1] / total;
    weight[2] = weight[2] / total;
    #endif

    // if (isValid && !isInside)
    // {
    //     if (x > 0 && y > 0 && bShouldRecur)
    //     {
    //         //isInside = false;
    //         tryToDrawPixel(x-10, y, isValid, isInside, 
    //                 v1, v2, v3, 
    //                 triangleVertices,
    //                 ww, uP, vP, cameraLocation, 
    //                 worldLoc, pTarget, lights, texture, false);
    //         tryToDrawPixel(x, y-10, isValid, isInside, 
    //                 v1, v2, v3, 
    //                 triangleVertices,
    //                 ww, uP, vP, cameraLocation, 
    //                 worldLoc, pTarget, lights, texture, false);
    //         tryToDrawPixel(x-10, y-10, isValid, isInside, 
    //                 v1, v2, v3, 
    //                 triangleVertices,
    //                 ww, uP, vP, cameraLocation, 
    //                 worldLoc, pTarget, lights, texture, false);

    //         pTarget->SetPixel(x-1, y-1, 0, Color(255, 0, 0, 255));
    //     }
    // }

    if (isValid)
    {   
        float intensity = 0.f;

        Vec3 p(0,0,0);

        //get color
        for (unsigned int i = 0; i < 3; i++)
        {
            p.x += weight[i] * worldLoc[i]->x;
            p.y += weight[i] * worldLoc[i]->y;
            p.z += weight[i] * worldLoc[i]->z;
        }

        //BECAREFUL
        if (!(x > 0 && x < pTarget->width && y > 0 && y < pTarget->height))
            return false;
        //BECAREFUL
        float currentDepth = pTarget->depthBuffer.getDepth(x, y);
        if (currentDepth <= p.z)
            return false;

        RasterizingVertex vert;
        vert.position3D = std::move(p);
        Color c(0, 0, 0, 0);

        float u = 0;
        float v = 0;

        //get color
        for (unsigned int i = 0; i < 3; i++)
        {
            c.r += weight[i] * triangleVertices[i]->color.r;
            c.g += weight[i] * triangleVertices[i]->color.g;
            c.b += weight[i] * triangleVertices[i]->color.b;
            c.a += weight[i] * triangleVertices[i]->color.a;

            vert.normal += weight[i] * triangleVertices[i]->normal;

            u += weight[i] * uP[i];
            v += weight[i] * vP[i];
        }

        // //unprecision of interpolation of char
        // if (triangleVertices[0]->color.a == 255 
        //     && triangleVertices[1]->color.a == 255 
        //     && triangleVertices[2]->color.a == 255)
        //     c.a = 255;
        
        vert.normal.Normalize();

        intensity = RenderTriangle::getPixelLight(vert, lights, cameraLocation);

        if (texture != nullptr && texture->pixels != nullptr)
        {
            //TODO : enum for interpolation type

            #ifdef __NEAREST_INTERPOLATION__
            {
                float alpha = c.a;
                assert(0 <= u && u <= 1 && 0 <= v && v <= 1);
                //nearest interpolation
                //max uv is 1, 1 * width = width, width isn't a valid index, so we substract by 1
                c = texture->GetPixelColor(static_cast<unsigned int>(u * (float(texture->width) - 1)), 
                                            static_cast<unsigned int>(v * (float(texture->height) - 1)));
    
                c.a = alpha;
            }
            #endif

            #ifdef __BILINEAR_INTERPOLATION__
            //bilinear interpolation
            {
                // floor
                // x1,y1--------x2,y1
                //   |            |
                //   |            |
                //   |  x,y       |
                //   |   #        |
                //   |            |
                // x1,y2--------x2,y2
                //               ceil

                float alpha = c.a;

                //TODO : assert ?
                //if not in range, due to float imprecision
                u = clamp(u, 0.f, 1.f);
                v = clamp(v, 0.f, 1.f);

                const int x1 = static_cast<int>(floor(u * (float(texture->width)-1)));
                const int x2 = static_cast<int>(ceil(u * (float(texture->width)-1)));

                const int y1 = static_cast<int>(floor(v * (float(texture->height)-1)));
                const int y2 = static_cast<int>(ceil(v * (float(texture->height)-1)));

                const float curX = u * (float(texture->width) - 1);
                const float curY = v * (float(texture->height) - 1);

                if (x2 - x1 == 0)
                {
                    Color c1 = texture->GetPixelColor(x1, y1);
                    Color c2 = texture->GetPixelColor(x1, y2);

                    c = getAverageColor(c2, c1, (curY - y1) / (y2 - y1));
                }
                else if (y2 - y1 == 0)
                {
                    Color c1 = texture->GetPixelColor(x1, y1);
                    Color c2 = texture->GetPixelColor(x2, y1);

                    c = getAverageColor(c2, c1, (curX - x1) / (x2 - x1));
                }
                else 
                {
                    Color c11 = texture->GetPixelColor(x1, y1);
                    Color c21 = texture->GetPixelColor(x2, y1);
                    Color c12 = texture->GetPixelColor(x1, y2);
                    Color c22 = texture->GetPixelColor(x2, y2);

                    Color c1 = getAverageColor(c21, c11, (curX - x1 / (x2 - x1)));
                    Color c2 = getAverageColor(c22, c12, (curX - x1 / (x2 - x1)));

                    c = getAverageColor(c2, c1, (curY - y1) / (y2 - y1));
                }

                c.a = alpha;
            }
            #endif
        }

        //c *= intensity;
        c.r *= intensity;
        c.g *= intensity;
        c.b *= intensity;

        //calcul z
        float z = (v1.z) * weight[0] + (v2.z) * weight[1] + (v3.z) * weight[2];
        //std::cout << c.getTransparence() << '\n';
        // if (c.a == 0)
        //     std::cout << c << '\n';
        pTarget->SetPixel(x, y, z, c);

        //isInside = true;
        //isValid = false;
    }
    return true;
    // else 
    // {
    //     // end of the triangle

    //     if (isInside && bShouldRecur)
    //     {
    //         isInside = false;
    //         tryToDrawPixel(x+1, y, isValid, isInside, 
    //                 v1, v2, v3, 
    //                 triangleVertices,
    //                 ww, uP, vP, cameraLocation, 
    //                 worldLoc, pTarget, lights, texture, false);
    //         tryToDrawPixel(x, y+1, isValid, isInside, 
    //                 v1, v2, v3, 
    //                 triangleVertices,
    //                 ww, uP, vP, cameraLocation, 
    //                 worldLoc, pTarget, lights, texture, false);
    //         tryToDrawPixel(x+1, y+1, isValid, isInside, 
    //                 v1, v2, v3, 
    //                 triangleVertices,
    //                 ww, uP, vP, cameraLocation, 
    //                 worldLoc, pTarget, lights, texture, false);

    //         //pTarget->SetPixel(x+1, y+1, 0, Color(255, 0, 0, 255));
    //     }
    //     //     break;
    // }
}

void drawTriangle(Vertex& vert1, Vertex& vert2, Vertex& vert3, Vec3 worldLoc1, Vec3 worldLoc2, Vec3 worldLoc3, 
                    float w1, float w2, float w3,
                    const Vec3& cameraLocation, FrameBuffer* pTarget, std::vector<Light>& lights, Texture* texture)
{
    std::array<Vertex*, 3> triangleVertices;
    triangleVertices[0] = &vert1;
    triangleVertices[1] = &vert2;
    triangleVertices[2] = &vert3;

    #ifdef __PERSPECTIVE_FIX__
    float ww[3];
    ww[0] = w1;
    ww[1] = w2;
    ww[2] = w3;
    #endif

    bool isValid = false;
    bool isInside = false;

    std::array<Vec3*, 3> worldLoc;
    worldLoc[0] = &worldLoc1;
    worldLoc[1] = &worldLoc2;
    worldLoc[2] = &worldLoc3;

    const Vec3& v1 = vert1.position;
    const Vec3& v2 = vert2.position;
    const Vec3& v3 = vert3.position;

    //getWeight constants
    unsigned int minX = std::min(std::min(v1.x, v2.x), v3.x);
    unsigned int maxX = std::max(std::max(v1.x, v2.x), v3.x);
    unsigned int minY = std::min(std::min(v1.y, v2.y), v3.y);
    unsigned int maxY = std::max(std::max(v1.y, v2.y), v3.y);

    //float weight[3];

    // float intensityVertex[3];
    // intensityVertex[0] = getPixelLight(*triangleVertices[0]);
    // intensityVertex[1] = getPixelLight(*triangleVertices[1]);
    // intensityVertex[2] = getPixelLight(*triangleVertices[2]);

    float uP[3] = {vert1.u,vert2.u,vert3.u};
    float vP[3] = {vert1.v,vert2.v,vert3.v};

    //BECAREFUL
    //clipping in rasterization
    if (minY < 0)
        minY = 0;
    if (minX < 0)
        minX = 0;
    if (maxY > pTarget->height)
        maxY = pTarget->height;
    if (maxX > pTarget->width)
        maxX = pTarget->width;

    #ifdef __MULTI_SAMPLING__
    // every pixel respecting (x % 2 == 0 && y % 2 == 0)) 
    // construct the base texture.
    minX -= minX % 2;
    minY -= minY % 2;
    maxX -= maxX % 2;
    maxY -= maxY % 2;
    #endif

    //TODO : set WeightVar outside of the loops
    #ifdef __MULTI_SAMPLING__
    for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
    {
        isInside = false;
        for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
        {
    #else 
    for (unsigned int y = minY; y <= maxY; y ++)
    {
        isInside = false;
        for (unsigned int x = minX; x <= maxX; x ++)
        {
    #endif

            tryToDrawPixel(x, y, isValid, isInside, 
                            v1, v2, v3, 
                            triangleVertices,
                            ww, uP, vP, cameraLocation, 
                            worldLoc, pTarget, lights, texture);
        }
    }
    
    #ifdef __MULTI_SAMPLING__

    bool bOld = false;
    for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
    {
        bOld = false;
        for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
        {
            float weight[3];
            bool bNew = getWeight(Vec2(x,y), v1, v2, v3, weight);
            // if (bNew != bOld)
            if (bNew && !bOld)
            {
                for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
                {
                    for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
                    {
                        const float currentX = x + (iAliasing - antiAliasingX);
                        const float currentY = y + (jAliasing - antiAliasingY);
                        if (!tryToDrawPixel(currentX, currentY, isValid, isInside, 
                                        v1, v2, v3, 
                                        triangleVertices,
                                        ww, uP, vP, cameraLocation, 
                                        worldLoc, pTarget, lights, texture))
                        {
                            if (pTarget->GetDepth(currentX, currentY) >= depthMax)
                            {
                                pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
                                #ifdef __ANTI_ALIASING_DEBUG__
                                pTarget->SetPixel(currentX, currentY, 0, Color(1, 0, 0, 1));
                                #endif
                            }
                        }
                    }
                }
            }
            if (!bNew && bOld)
            {
                for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
                {
                    for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
                    {
                        const float currentX = x + (iAliasing - antiAliasingX);
                        const float currentY = y + (jAliasing - antiAliasingY);
                        if (!tryToDrawPixel(currentX, currentY, isValid, isInside, 
                                        v1, v2, v3, 
                                        triangleVertices,
                                        ww, uP, vP, cameraLocation, 
                                        worldLoc, pTarget, lights, texture))
                        {
                            if (pTarget->GetDepth(currentX, currentY) >= depthMax)
                            {
                                pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
                                #ifdef __ANTI_ALIASING_DEBUG__
                                pTarget->SetPixel(currentX, currentY, 0, Color(0, 1, 0, 1));
                                #endif
                            }
                        }
                    }
                }
            }
            bOld = bNew;
        }
    }

    for (unsigned int y = minY; y <= maxY; y += antiAliasingY)
    {
        bOld = false;
        for (unsigned int x = minX; x <= maxX; x += antiAliasingX)
        {
            float weight[3];
            bool bNew = getWeight(Vec2(x,y), v1, v2, v3, weight);
            if (bNew && !bOld)
            {
                for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
                {
                    for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
                    {
                        const float currentX = x + (iAliasing - antiAliasingX);
                        const float currentY = y + (jAliasing - antiAliasingY);
                        if (!tryToDrawPixel(currentX, currentY, isValid, isInside, 
                                        v1, v2, v3, 
                                        triangleVertices,
                                        ww, uP, vP, cameraLocation, 
                                        worldLoc, pTarget, lights, texture))
                        {
                            if (pTarget->GetDepth(currentX, currentY) >= depthMax)
                            {
                                pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
                                #ifdef __ANTI_ALIASING_DEBUG__
                                pTarget->SetPixel(currentX, currentY, 0, Color(0, 0, 1, 1));
                                #endif
                            }
                        }
                    }
                }
            }
            if (!bNew && bOld)
            {
                for (unsigned int iAliasing = 0; iAliasing < antiAliasingX*2; iAliasing++)
                {
                    for (unsigned int jAliasing = 0; jAliasing < antiAliasingY*2; jAliasing++)
                    {
                        const float currentX = x + (iAliasing - antiAliasingX);
                        const float currentY = y + (jAliasing - antiAliasingY);
                        // if (currentX > minX && currentY > minY && currentX < maxX && currentY < maxY)
                        // {
                            if (!tryToDrawPixel(currentX, currentY, isValid, isInside, 
                                            v1, v2, v3, 
                                            triangleVertices,
                                            ww, uP, vP, cameraLocation, 
                                            worldLoc, pTarget, lights, texture))
                            {
                                if (pTarget->GetDepth(currentX, currentY) >= depthMax)
                                {
                                    pTarget->depthBuffer.setDepth(currentX, currentY, depthMax - 1);
                                    #ifdef __ANTI_ALIASING_DEBUG__
                                    pTarget->SetPixel(currentX, currentY, 0, Color(1, 1, 0, 1));
                                    #endif
                                }
                            }
                        //}
                    }
                }
            }
            bOld = bNew;
        }
    }
    #endif
}
