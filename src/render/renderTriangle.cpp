#include "renderTriangle.h"

#include "rasterizer.h"
#include "frameBuffer.h"
#include "vertex.h"
#include "rasterizingVertex.h"
#include "color.h"

constexpr int deltaPixelsX = 5; 
constexpr int deltaPixelsY = 1; 

RenderTriangle::RenderTriangle(const std::array<RasterizingVertex, 3>& triangleVertices, FrameBuffer* pTarget)
    : triangleVertices({&triangleVertices[0], &triangleVertices[1], &triangleVertices[2]}),
      pTarget(pTarget)
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

void RenderTriangle::draw() 
{
    const Vec3& v1 = triangleVertices[0]->position2D;//Rasterizer::projection(triangleVertices[0]->position);
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

                //get color
                for (unsigned int i = 0; i < 3; i++)
                {
                    c.r += weight[i] * triangleVertices[i]->color.r;
                    c.g += weight[i] * triangleVertices[i]->color.g;
                    c.b += weight[i] * triangleVertices[i]->color.b;
                }

                //calcul z
                float z = (v1.z) * weight[0] + (v2.z) * weight[1] + (v3.z) * weight[2];

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
