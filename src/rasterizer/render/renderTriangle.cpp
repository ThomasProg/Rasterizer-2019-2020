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

float RenderTriangle::getPixelLight(const RasterizingVertex& vertex, const std::vector<Light>& lights)
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
        float cosAlpha = dotProduct(vertex.normal, h/h.GetMagnitude());

        if (cosAlpha < 0)
            cosAlpha = 0;

        specular = light.specularComponent * mat.specular*std::pow(cosAlpha,mat.brillance);

        total += ambient + diffuse + specular;
    }

    return total;
}

////////Get weight of a point in a triangle//////////
bool getWeight(const Vec2& p, Vec3 p1, Vec3 p2, Vec3 p3, float* weight)
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

    // weight[0] = (cst[0] * (p.x - p3.x) + cst[1] * (p.y - p3.y)) / det;
    // weight[1] = (cst[2] * (p.x - p3.x) + cst[3] * (p.y - p3.y)) / det;
    // weight[2] = 1 - weight[1] - weight[0];
    
    return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
}

void drawTriangle(Vertex vert1, Vertex vert2, Vertex vert3, FrameBuffer* pTarget, std::vector<Light>& lights)
{
    std::array<Vertex*, 3> triangleVertices;
    triangleVertices[0] = &vert1;
    triangleVertices[1] = &vert2;
    triangleVertices[2] = &vert3;

    const Vec3& v1 = vert1.position;
    const Vec3& v2 = vert2.position;
    const Vec3& v3 = vert3.position;

    //getWeight constants
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
            float weight[3];
            isValid = getWeight(Vec2(x,y), v1, v2, v3, weight);

                //std::cout << "grs" << std::endl;
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

                    p.x += weight[i] * triangleVertices[i]->position.x;
                    p.y += weight[i] * triangleVertices[i]->position.y;
                    p.z += weight[i] * triangleVertices[i]->position.z;

                    vert.normal += weight[i] * triangleVertices[i]->normal;

                    u += weight[i] * uP[i];
                    v += weight[i] * vP[i];

                    //intensity += weight[i] * intensityVertex[i];
                }
                //std::cout << "u : " << static_cast<unsigned int>(u * float(texture->width)) << std::endl;
                vert.normal.Normalize();
                //vert.normal = crossProduct(v3-v1,v2-v1);
                intensity = RenderTriangle::getPixelLight(vert, lights);
                
                // c = texture->GetPixelColor(static_cast<unsigned int>(u * float(texture->width)), 
                //                            static_cast<unsigned int>(v * float(texture->height)));

                c *= intensity;
                
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