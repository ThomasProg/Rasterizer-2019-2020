#include "renderTriangle.h"

#include "frameBuffer.h"
#include "vertex.h"
#include "color.h"

// RenderTriangle::RenderTriangle(const std::array<Vertex const * const, 3> triangleVertices)
//     : triangleVertices(triangleVertices)
// {

// }

RenderTriangle::RenderTriangle(const std::array<const Vertex*, 3>& triangleVertices, FrameBuffer* pTarget)
    : triangleVertices({triangleVertices[0], triangleVertices[1], triangleVertices[2]}),
      pTarget(pTarget)
{

}

// RenderTriangle::RenderTriangle(Vertex const * const v1, Vertex const * const v2, Vertex const * const v3)
//     : triangleVertices({v1, v2, v3})
// {

// }



////////Get weight of a point in a triangle//////////
bool RenderTriangle::getWeight(const Vec2& p, float* weight) const
{
    const Vec2& p1 = triangleVertices[0]->position;
    const Vec2& p2 = triangleVertices[1]->position;
    const Vec2& p3 = triangleVertices[2]->position;

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

Color RenderTriangle::getColorAtPixel(Vec2 p, bool& isValid, float* weight) const
{
    Color c(0,0,0);
    isValid = getWeight(p, weight);

    for (unsigned int i = 0; i < 3; i++)
    {
        c.r += weight[i] * triangleVertices[i]->color.r;
        c.g += weight[i] * triangleVertices[i]->color.g;
        c.b += weight[i] * triangleVertices[i]->color.b;
    }

    return c;
}


///////draw an horizontal line//////// O(n)
void RenderTriangle::drawHorizontalLine(Vec2 p1, Vec2 p2, int height, 
                                        const std::array<const Vertex*, 3>& triangleVertices) const
{
    Color c;
    bool isValid;
    float weight[3];

    if (p1.x > p2.x)
        for (int x = p2.x; x < p1.x; x++)
        {
            c = getColorAtPixel(Vec2(x, height), isValid, weight);
            if (!isValid)
                continue;

            float depth;
            pTarget->SetPixel(x, height, p1.y, c);
        }
    else 
        for (int x = p1.x; x < p2.x; x++)
        {
            c = getColorAtPixel(Vec2(x, height), isValid, weight);
            if (!isValid)
                continue;

            float depth;
            pTarget->SetPixel(x, height, p1.y, c);
        }
}

// //TODO : replace Vec2 by Vertex
// /* 
//  * if used, draw vertices as BottomFlatTriangle
//  * Complexity : O(n^2)
//  */
void RenderTriangle::fillBottomFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices) const
{
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float invslopeZ1 = (v2.z - v1.z) / (v2.y - v1.y);
    float invslopeZ2 = (v3.z - v1.z) / (v3.y - v1.y);

    //Vec2 xMinMax (v1.x, v1.x);

    Vec2 p1(v1.x, v1.z), p2(v1.x, v1.z);

    for (float verticalLoc = v1.y; verticalLoc <= v2.y + 1; verticalLoc++)
    {
        drawHorizontalLine(p1, p2, verticalLoc, triangleVertices);
        p1.x += invslope1;
        p2.x += invslope2;

        p1.y += invslopeZ1;
        p2.y += invslopeZ2;
    }
}

//TODO : replace Vec2 by Vertex
/* 
 * if used, draw vertices as BottomFlatTr    // A = v1;
    // B = v2;
    // C = v3;iangle
 * Complexity : O(n^2)
 */
void RenderTriangle::fillTopFlatTriangle(const std::array<const Vertex *, 3>& triangleVertices) const
{
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    // float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    // float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);
    float invslope1 = (v1.x - v3.x) / (v1.y - v3.y);
    float invslope2 = (v2.x - v3.x) / (v2.y - v3.y);

    float invslopeZ1 = (v2.z - v1.z) / (v3.y - v1.y);
    float invslopeZ2 = (v3.z - v2.z) / (v3.y - v2.y);

    Vec2 p1(v3.x, v3.z), p2(v3.x, v3.z);

    for (float verticalLoc = v3.y - 1; verticalLoc > v1.y; verticalLoc--)
    {
        drawHorizontalLine(p1, p2, verticalLoc, triangleVertices);
        p1.x -= invslope1;
        p2.x -= invslope2;

        p1.y -= invslopeZ1;
        p2.y -= invslopeZ2;
    }
}

// void RenderTriangle::draw() const
// {
//     const Vec3& v1 = triangleVertices[0]->position;
//     const Vec3& v2 = triangleVertices[1]->position;
//     const Vec3& v3 = triangleVertices[2]->position;

//     /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
//     //sortVerticesAscendingByY();
//     //if (v1.y > v2.y && v1.y > )

//     /* here we know that v1.y <= v2.y <= v3.y */
//     /* check for trivial case of bottom-flat triangle */
//     if (v2.y == v3.y)
//     {
//         fillBottomFlatTriangle(triangleVertices);
//     }
//     /* check for trivial case of top-flat triangle */
//     else if (v1.y == v2.y)
//     {
//         fillTopFlatTriangle(triangleVertices);
//     }
//     else
//     {
//         /* general case - split the triangle in a topflat and bottom-flat one */
//         Vertex vertex4 (Vec3(
//             (v1.x + ((v2.y - v1.y) / (v3.y - v1.y)) * (v3.x - v1.x)), 
//             v2.y, 
//             v2.z)); //TODO : check if z = v2.z is correct

//         //vertex4.color = getColorAtPixel(vertex4.position, triangleVertices);

//         std::array<const Vertex*, 3> bottomTriangleVertices;
//         bottomTriangleVertices[0] = triangleVertices[0];
//         bottomTriangleVertices[1] = triangleVertices[1];
//         bottomTriangleVertices[2] = &vertex4;

//         std::array<const Vertex*, 3> topTriangleVertices;
//         topTriangleVertices[0] = triangleVertices[1];
//         topTriangleVertices[1] = &vertex4;
//         topTriangleVertices[2] = triangleVertices[2];
//         fillBottomFlatTriangle(bottomTriangleVertices);
//         fillTopFlatTriangle(topTriangleVertices);
//     }
// }

void RenderTriangle::draw() const
{
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

    int minX = std::min(std::min(v1.x, v2.x), v3.x);
    int maxX = std::max(std::max(v1.x, v2.x), v3.x);
    int minY = std::min(std::min(v1.y, v2.y), v3.y);
    int maxY = std::max(std::max(v1.y, v2.y), v3.y);

    //TODO : set WeightVar outside of the loops
    for (int x = minX-1; x <= maxX; x++)
    {
        for (int y = minY-1; y <= maxY; y++)
        {
            bool isValid;
            float weight[3];
            Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
            float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];
            // if (v1.z > 0.5)
            // std::cout << v1.z << std::endl;
            // if (z < -3.0)
            // std::cout << z << std::endl;
            // c.r = z*255;
            // c.g = z*255;
            // c.b = z*255;

            if (isValid)
            {
                pTarget->SetPixel(x, y, z, c);
            }
        }
    }
}
