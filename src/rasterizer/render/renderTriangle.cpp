#include <cmath>
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

    // weight[0] = (cst[0] * (p.x - p3.x) + cst[1] * (p.y - p3.y)) / det;
    // weight[1] = (cst[2] * (p.x - p3.x) + cst[3] * (p.y - p3.y)) / det;
    // weight[2] = 1 - weight[1] - weight[0];
    
    return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
}

void drawTriangle(Vertex& vert1, Vertex& vert2, Vertex& vert3, Vec3 worldLoc1, Vec3 worldLoc2, Vec3 worldLoc3, 
                    const Vec3& cameraLocation, FrameBuffer* pTarget, std::vector<Light>& lights, Texture* texture)
{
    std::array<Vertex*, 3> triangleVertices;
    triangleVertices[0] = &vert1;
    triangleVertices[1] = &vert2;
    triangleVertices[2] = &vert3;

    std::array<Vec3*, 3> worldLoc;
    worldLoc[0] = &worldLoc1;
    worldLoc[1] = &worldLoc2;
    worldLoc[2] = &worldLoc3;

    //TODO : FIX TEXTURE 
    // for (unsigned int i = 0; i < 3; i++)
    // {
    //     triangleVertices[i]->position /= worldLoc[i]->z;
    //     worldLoc[i]->z = 1 / worldLoc[i]->z;
    // }


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
                Color c(0, 0, 0, 0);

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
                    continue;
                //BECAREFUL
                float currentDepth = pTarget->depthBuffer.getDepth(x, y);
                if (currentDepth <= p.z)
                    continue;

                RasterizingVertex vert;
                vert.position3D = std::move(p);

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

                    //intensity += weight[i] * intensityVertex[i];
                }

                // u /= p.z;
                // v /= p.z;

                //unprecision of interpolation of char
                if (triangleVertices[0]->color.a == 255 
                    && triangleVertices[1]->color.a == 255 
                    && triangleVertices[2]->color.a == 255)
                    c.a = 255;
                
                vert.normal.Normalize();

                intensity = RenderTriangle::getPixelLight(vert, lights, cameraLocation);

                if (texture != nullptr && texture->pixels != nullptr)
                {
                    //TODO : enum for interpolation type

                    {
                        char alpha = c.a;
                        assert(0 <= u && u <= 1 && 0 <= v && v <= 1);
                        //nearest interpolation
                        //max uv is 1, 1 * width = width, width isn't a valid index, so we substract by 1
                        c = texture->GetPixelColor(static_cast<unsigned int>(u * (float(texture->width) - 1)), 
                                                   static_cast<unsigned int>(v * (float(texture->height) - 1)));
                        c.a = alpha;
                    }

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

                        char alpha = c.a;

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
                }

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