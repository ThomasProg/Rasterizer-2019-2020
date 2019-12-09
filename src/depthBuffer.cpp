#include <cassert>
#include "depthBuffer.h"

#include "macros.h"

DepthBuffer::DepthBuffer(unsigned int width, unsigned int height) 
    : width(width),
      height(height),
      depth(new float[width * height])
{
    
}

DepthBuffer::~DepthBuffer()
{
    if (depth != nullptr)
        delete[] depth;
}
// #include <cstring>
void DepthBuffer::resetDepth()
{
    //TODO : clean
    unsigned int size = width * height;
    for (unsigned int i = size-1; i--;)//i < size; i++)
    {
        depth[i] = depthMax;
    }

    // for (unsigned int i = 0; i < size; i++)//i < size; i++)
    // {
    //     depth[i] = depthMax;
    // }
    return;

    //loop unrolling
    if (size % 2 != 0)
    {
        depth[0] = depthMax;
        for (unsigned int i = 1; i < size; i+=2)
        {
            depth[i] = depthMax;
            depth[i+1] = depthMax;
        }
    }
    else 
        for (unsigned int i = 0; i < size; i+=2)
        {
            depth[i] = depthMax;
            depth[i+1] = depthMax;
        }
    //this code is equivalent to this :
    //
    // for (unsigned int i = 0; i < size; i++)
    // {
    //     depth[i] = depthMax;
    // }
}

float DepthBuffer::getDepth(unsigned int x, unsigned int y) const
{
    //assert(0 <= x && x < width && 0 <= y && y < height);
    if (0 <= x && x < width && 0 <= y && y < height)
        return depth[x + y * width];   
    else 
        return -1;
}

void DepthBuffer::setDepth(unsigned int x, unsigned int y, float newDepth)
{
    //assert(0 <= x && x < width && 0 <= y && y < height);
    //if (newDepth < -1 || newDepth > 1)

    if (0 <= x && x < width && 0 <= y && y < height)
        depth[x + y * width] = newDepth;
}