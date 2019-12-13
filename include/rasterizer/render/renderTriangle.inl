#include <cassert>
#include <algorithm>
#include "renderTriangle.h"

#include "vertex.h"
#include "texture.h"

__inline
void getUntexturedPixelColor(const std::array<Vertex, 3>& triangleVertices, 
                             const std::array<float, 3>& weight, 
                             Color& colorOutput)
{
    // Could be unrolled for optimization
    for (unsigned int i = 0; i < 3; i++)
    {
        colorOutput.r += weight[i] * triangleVertices[i].color.r;
        colorOutput.g += weight[i] * triangleVertices[i].color.g;
        colorOutput.b += weight[i] * triangleVertices[i].color.b;
        colorOutput.a += weight[i] * triangleVertices[i].color.a;
    }
}

__inline
void getTexturedColorNearestInterpolation(const Texture* texture, 
                                          const float u,
                                          const float v, 
                                          Color& colorOutput)
{
    // u and v represents a pixel location in the  texture, and them must be between 0 and 1.
    assert(0 <= u && u <= 1 && 0 <= v && v <= 1);

    // Max uv is 1, 1 * width = width, width isn't a valid index, 
    // so we substract by 1 before the multiplication.
    // since (u, v) >= 0, we can cast them to unsigned int without errors.
    colorOutput.copyRGB(texture->GetPixelColor(static_cast<unsigned int>(u * (float(texture->width) - 1)), 
                                               static_cast<unsigned int>(v * (float(texture->height) - 1))));
}

__inline
void getTexturedColorBilinearInterpolation(const Texture* texture, 
                                           const float u,
                                           const float v, 
                                           Color& colorOutput)
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

    // u and v represents a pixel location in the  texture, and them must be between 0 and 1.
    assert(0 <= u && u <= 1 && 0 <= v && v <= 1);

    const unsigned int x1 = static_cast<unsigned int>(floor(u * (float(texture->width)-1)));
    const unsigned int x2 = std::min(static_cast<unsigned int>(ceil(u * (float(texture->width)-1))), texture->width - 1);

    const unsigned int y1 = static_cast<unsigned int>(floor(v * (float(texture->height)-1)));
    const unsigned int y2 = std::min(static_cast<unsigned int>(ceil(v * (float(texture->height)-1))), texture->height - 1);

    const float curX = u * (float(texture->width) - 1);
    const float curY = v * (float(texture->height) - 1);

    //can't interpolate in case interpolating points are the same
    if (x2 == x1 && y2 == y1)
    {
        colorOutput.reset();
        return;
    }
    //in case the two interpolating points are on the same horizontal line
    else if (x2 == x1)
    {
        const Color c1 = texture->GetPixelColor(x1, y1);
        const Color c2 = texture->GetPixelColor(x1, y2);

        colorOutput.copyRGB(getAverageColor(c2, c1, (curY - y1) / (y2 - y1)));
    }
    //in case the two interpolating points are on the same vertical line
    else if (y2 == y1)
    {
        const Color c1 = texture->GetPixelColor(x1, y1);
        const Color c2 = texture->GetPixelColor(x2, y1);

        colorOutput.copyRGB(getAverageColor(c2, c1, (curX - x1) / (x2 - x1)));
    }
    //general case : 
    else 
    {
        const Color c11 = texture->GetPixelColor(x1, y1);
        const Color c21 = texture->GetPixelColor(x2, y1);
        const Color c12 = texture->GetPixelColor(x1, y2);
        const Color c22 = texture->GetPixelColor(x2, y2);

        const Color c1 = getAverageColor(c21, c11, (curX - x1 / (x2 - x1)));
        const Color c2 = getAverageColor(c22, c12, (curX - x1 / (x2 - x1)));

        colorOutput.copyRGB(getAverageColor(c2, c1, (curY - y1) / (y2 - y1)));
    }
}