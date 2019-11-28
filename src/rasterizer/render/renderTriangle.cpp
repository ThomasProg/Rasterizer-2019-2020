#include <cmath>
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

constexpr int deltaPixelsX = 5; 
constexpr int deltaPixelsY = 1; 

RenderTriangle::RenderTriangle(const std::array<RasterizingVertex, 3>& triangleVertices, 
                                FrameBuffer* pTarget, const std::vector<Light>& lights, Texture* texture)
    : triangleVertices({&triangleVertices[0], &triangleVertices[1], &triangleVertices[2]}),
      pTarget(pTarget),
      lights(lights),
      texture(texture)
{

}



////////Get weight of a point in a triangle//////////
bool RenderTriangle::getWeight(const Vec2& p, float* weight) const
{
    const Vec2& p1 = triangleVertices[0]->position2D;
    const Vec2& p2 = triangleVertices[1]->position2D;
    const Vec2& p3 = triangleVertices[2]->position2D;

    //float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    if (det == 0)
    {
        weight[0] = 0;
        weight[1] = 0;
        weight[2] = 0;
        return false;
    }

    // weight[0] = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
    // weight[1] = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
    // weight[2] = 1 - weight[1] - weight[0];

    weight[0] = (cst[0] * (p.x - p3.x) + cst[1] * (p.y - p3.y)) / det;
    weight[1] = (cst[2] * (p.x - p3.x) + cst[3] * (p.y - p3.y)) / det;
    weight[2] = 1 - weight[1] - weight[0];
    
    return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
}

float RenderTriangle::getPixelLight(const RasterizingVertex& vertex) const
{
    Material mat;

    float ambient  = 0.f;
    float diffuse  = 0.f;
    float specular = 0.f;  
    float total    = 0.f;

    for (const Light& light : lights)
    {
        ambient = light.ambientComponent * mat.ambient;

        Vec3 pixelLightDist = (light.position-vertex.position3D);
        pixelLightDist.Normalize();
        
        float cosTeta  = dotProduct(pixelLightDist,(vertex.normal));
        
        if (cosTeta < 0)
            cosTeta = 0;

        diffuse = light.diffuseComponent * mat.diffuse * cosTeta;

        Vec3 eyeToPixelVec = vertex.position3D; //TODO
        eyeToPixelVec.z -= 1;
        eyeToPixelVec.Normalize();
        Vec3 h = pixelLightDist+eyeToPixelVec;
        h.Normalize();
        float cosAlpha = dotProduct(vertex.normal,h/h.GetMagnitude());

        if (cosAlpha < 0)
            cosAlpha = 0;

        specular = light.specularComponent * mat.specular*std::pow(cosAlpha,mat.brillance);

        total += ambient + diffuse + specular;
    }

    return total;
}

void RenderTriangle::draw() 
{
    const Vec3& v1 = triangleVertices[0]->position2D;
    const Vec3& v2 = triangleVertices[1]->position2D;
    const Vec3& v3 = triangleVertices[2]->position2D;

    //getWeight constants
    det = (v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y);
    cst[0] = (v2.y - v3.y);
    cst[1] = (v3.x - v2.x);
    cst[2] = (v3.y - v1.y);
    cst[3] = (v1.x - v3.x);

    int minX = std::min(std::min(v1.x, v2.x), v3.x);
    int maxX = std::max(std::max(v1.x, v2.x), v3.x);
    int minY = std::min(std::min(v1.y, v2.y), v3.y);
    int maxY = std::max(std::max(v1.y, v2.y), v3.y);

    bool isInside = false;
    bool isValid = false;
    float weight[3];

    // float intensityVertex[3];
    // intensityVertex[0] = getPixelLight(*triangleVertices[0]);
    // intensityVertex[1] = getPixelLight(*triangleVertices[1]);
    // intensityVertex[2] = getPixelLight(*triangleVertices[2]);

    float uP[3] = {0,1,0};
    float vP[3] = {0,0,1};

    //TODO : set WeightVar outside of the loops
    for (int y = minY; y <= maxY; y++)
    {
        isInside = false;
        for (int x = minX; x <= maxX; x++)
        {
            isValid = getWeight(Vec2(x,y), weight);

            if (isValid)
            {
                Color c(0,0,0);

                float intensity = 0.f;

                 Vec3 p(0,0,0);

                RasterizingVertex vert;
                vert.position3D = p;

                float u = 0;
                float v = 0;

                //get color
                for (unsigned int i = 0; i < 3; i++)
                {
                    c.r += weight[i] * triangleVertices[i]->color.r;
                    c.g += weight[i] * triangleVertices[i]->color.g;
                    c.b += weight[i] * triangleVertices[i]->color.b;

                    p.x += weight[i] * triangleVertices[i]->position3D.x;
                    p.y += weight[i] * triangleVertices[i]->position3D.y;
                    p.z += weight[i] * triangleVertices[i]->position3D.z;

                    vert.normal += weight[i] * triangleVertices[i]->normal;

                    u += weight[i] * uP[i];
                    v += weight[i] * vP[i];

                    //intensity += weight[i] * intensityVertex[i];
                }
                //std::cout << "u : " << static_cast<unsigned int>(u * float(texture->width)) << std::endl;
                vert.normal.Normalize();
                //vert.normal = crossProduct(v3-v1,v2-v1);
                intensity = getPixelLight(vert);
                
                c = texture->GetPixelColor(static_cast<unsigned int>(u * float(texture->width)), 
                                           static_cast<unsigned int>(v * float(texture->height)));

                c *= intensity;

                // if (intensity < 0)
                //     intensity = 0;

                // //since -1 < intensity < 1, we need to lerp the intensity for the color
                // if (c.r > 255.f / intensity)
                //     c.r = 255;
                // else 
                //     c.r *= intensity;
                    
                // if (c.g > 255.f / intensity)
                //     c.g = 255;
                // else 
                //     c.g *= intensity;

                // if (c.b > 255.f / intensity)
                //     c.b = 255;       
                // else 
                //     c.b *= intensity;    
                
                //calcul z
                float z = (v1.z) * weight[0] + (v2.z) * weight[1] + (v3.z) * weight[2];
                
                //c *= z;

                pTarget->SetPixel(x, y, z, c);

                isInside = true;
                isValid = false;
            }
            else 
            {
                if (isInside)
                    break;
            }
        }
    }
}
