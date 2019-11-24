#include "renderTriangle.h"

#include "frameBuffer.h"
#include "vertex.h"
#include "color.h"

constexpr int deltaPixelsX = 5; 
constexpr int deltaPixelsY = 1; 

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


// ///////draw an horizontal line//////// O(n)
// void RenderTriangle::drawHorizontalLine(Vec2 p1, Vec2 p2, int height, 
//                                         const std::array<const Vertex*, 3>& triangleVertices) const
// {
//     const Vec3& v1 = triangleVertices[0]->position;
//     const Vec3& v2 = triangleVertices[1]->position;
//     const Vec3& v3 = triangleVertices[2]->position;
    
//     Color c;
//     bool isValid;
//     float weight[3];

//     if (p1.x > p2.x)
//         for (int x = p2.x - 2; x < p1.x+2; x++)
//         {
//             c = getColorAtPixel(Vec2(x, height), isValid, weight);
//             if (!isValid)
//                 continue;
// float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];
//             float depth;
//             pTarget->SetPixel(x, height, z, c);
//         }
//     else 
//         for (int x = p1.x - 2; x < p2.x; x++)
//         {
//             c = getColorAtPixel(Vec2(x, height), isValid, weight);
//             if (!isValid)
//                 continue;
// float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];
//             float depth;
//             pTarget->SetPixel(x, height, z, c);
//         }
// }

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

    const Vec3& v11 = this->triangleVertices[0]->position;
    const Vec3& v22 = this->triangleVertices[1]->position;
    const Vec3& v33 = this->triangleVertices[2]->position;

    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    //Vec2 xMinMax (v1.x, v1.x);

    Vec2 p1(v1.x, v1.z), p2(v1.x, v1.z);

    for (int y = v1.y - deltaPixelsY; y <= v2.y + deltaPixelsY; y++)
    {
        //drawHorizontalLine(p1, p2, verticalLoc, triangleVertices);
        int xMin = std::min(p1.x, p2.x);
        int xMax = std::max(p1.x, p2.x);
        for (int x = xMin - deltaPixelsX; x <= xMax + deltaPixelsX; x++)
        {
            bool isValid;
            float weight[3];
            Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
            float z = (v11.z+0.5) * weight[0] + (v22.z+0.5) * weight[1] + (v33.z+0.5) * weight[2];

            if (isValid)
            {
                pTarget->SetPixel(x, y, z, c);
            }
        }

        p1.x += invslope1;
        p2.x += invslope2;
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

    const Vec3& v11 = this->triangleVertices[0]->position;
    const Vec3& v22 = this->triangleVertices[1]->position;
    const Vec3& v33 = this->triangleVertices[2]->position;

    // float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    // float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);
    float invslope1 = (v1.x - v3.x) / (v1.y - v3.y);
    float invslope2 = (v2.x - v3.x) / (v2.y - v3.y);

    Vec2 p1(v3.x, v3.z), p2(v3.x, v3.z);

    for (int y = v3.y + deltaPixelsY; y > v1.y - deltaPixelsY; y--)
    {
        //drawHorizontalLine(p1, p2, verticalLoc, triangleVertices);
        int xMin = std::min(p1.x, p2.x);
        int xMax = std::max(p1.x, p2.x);
        for (int x = xMin - deltaPixelsX; x <= xMax + deltaPixelsX; x++)
        {
            bool isValid;
            float weight[3];
            Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
            float z = (v11.z+0.5) * weight[0] + (v22.z+0.5) * weight[1] + (v33.z+0.5) * weight[2];

            if (isValid)
            {
                pTarget->SetPixel(x, y, z, c);
            }
        }

        p1.x -= invslope1;
        p2.x -= invslope2;
    }
}

// void RenderTriangle::fillBottomFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices) const
// {
//     const Vec3& v1 = triangleVertices[0]->position;
//     const Vec3& v2 = triangleVertices[1]->position;
//     const Vec3& v3 = triangleVertices[2]->position;

//     int minX = std::min(std::min(v1.x, v2.x), v3.x);
//     int maxX = std::max(std::max(v1.x, v2.x), v3.x);
//     int minY = std::min(std::min(v1.y, v2.y), v3.y);
//     int maxY = std::max(std::max(v1.y, v2.y), v3.y);

//     for (int x = minX; x < maxX; x++)
//     {
//         for (int y = minY; y < maxY; y++)
//         {
//             bool isValid;
//             float weight[3];
//             Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
//             float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];
//             // if (v1.z > 0.5)
//             // std::cout << v1.z << std::endl;
//             // if (z < -3.0)
//             // std::cout << z << std::endl;
//             // c.r = z*255;
//             // c.g = z*255;
//             // c.b = z*255;

//             if (isValid)
//             {
//                 pTarget->SetPixel(x, y, z, c);
//             }
//         }
//     }
// }

// void RenderTriangle::fillTopFlatTriangle(const std::array<const Vertex *, 3>& triangleVertices) const
// {
//     const Vec3& v1 = triangleVertices[0]->position;
//     const Vec3& v2 = triangleVertices[1]->position;
//     const Vec3& v3 = triangleVertices[2]->position;

//     int minX = std::min(std::min(v1.x, v2.x), v3.x);
//     int maxX = std::max(std::max(v1.x, v2.x), v3.x);
//     int minY = std::min(std::min(v1.y, v2.y), v3.y);
//     int maxY = std::max(std::max(v1.y, v2.y), v3.y);

//     // for (int x = minX; x < maxX; x++)
//     // {
//     //     for (int y = maxY; y < minY; y++)
//     //     {
//     //         bool isValid;
//     //         float weight[3];
//     //         Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
//     //         float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];
//     //         // if (v1.z > 0.5)
//     //         // std::cout << v1.z << std::endl;
//     //         // if (z < -3.0)
//     //         // std::cout << z << std::endl;
//     //         // c.r = z*255;
//     //         // c.g = z*255;
//     //         // c.b = z*255;

//     //         if (isValid)
//     //         {
//     //             pTarget->SetPixel(x, y, z, c);
//     //         }
//     //     }
//     // }
// }

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

void RenderTriangle::draw() 
{
    const Vec3& v1 = triangleVertices[0]->position;
    const Vec3& v2 = triangleVertices[1]->position;
    const Vec3& v3 = triangleVertices[2]->position;

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
            //Color c = getColorAtPixel(Vec2(x, y), isValid, weight);

            isValid = getWeight(Vec2(x,y), weight);

            // if (v1.z > 0.5)
            // std::cout << v1.z << std::endl;
            // if (z < -3.0)
            // std::cout << z << std::endl;
            // c.r = z*255;
            // c.g = z*255;
            // c.b = z*255;

            if (isValid)
            {
            Color c(0,0,0);

            for (unsigned int i = 0; i < 3; i++)
            {
                c.r += weight[i] * triangleVertices[i]->color.r;
                c.g += weight[i] * triangleVertices[i]->color.g;
                c.b += weight[i] * triangleVertices[i]->color.b;
            }

            float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];
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

//         Vertex vertex4 (Vec3(
//             (v1.x + ((v2.y - v1.y) / (v3.y - v1.y)) * (v3.x - v1.x)), 
//             v2.y, 
//             0)); 

// // Color c(255, 255, 255);
// // pTarget->SetPixel(vertex4.position.x, vertex4.position.y, 0, c);

//     {
//     float invslope1 = (v1.x - v3.x) / (v1.y - v3.y);
//     float invslope2 = (v2.x - v3.x) / (v2.y - v3.y);

//     Vec2 p1(v3.x, v3.z), p2(v3.x, v3.z);

//     for (int y = v3.y; y > v1.y; y--)
//     {
//         //drawHorizontalLine(p1, p2, verticalLoc, triangleVertices);
//         for (int x = p1.x; x < p2.x; x++)
//         {
//             bool isValid;
//             float weight[3];
//             Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
//             float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];

//             if (isValid)
//             {
//                 pTarget->SetPixel(x, y, z, c);
//             }
//         }
//         p1.x -= invslope1;
//         p2.x -= invslope2;
//     }
//     }

//     {
//     float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
//     float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

//     Vec2 p1(v1.x, v1.z), p2(v1.x, v1.z);

//     for (int y = v1.y - 3; y <= v2.y + 3; y++)
//     {
//         //drawHorizontalLine(p1, p2, verticalLoc, triangleVertices);
//         for (int x = p1.x; x < p2.x; x++)
//         {
//             bool isValid;
//             float weight[3];
//             Color c = getColorAtPixel(Vec2(x, y), isValid, weight);
//             float z = (v1.z+0.5) * weight[0] + (v2.z+0.5) * weight[1] + (v3.z+0.5) * weight[2];

//             if (isValid)
//             {
//                 pTarget->SetPixel(x, y, z, c);
//             }
//         }
//         p1.x += invslope1;
//         p2.x += invslope2;
//     }
//     }
}
