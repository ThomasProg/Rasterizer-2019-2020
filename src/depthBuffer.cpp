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

void DepthBuffer::resetDepth()
{
    unsigned int size = width * height;
    for (unsigned int i = size-1; i--;)
    {
        depth[i] = depthMax;
    }
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
    if (0 <= x && x < width && 0 <= y && y < height)
        depth[x + y * width] = newDepth;
}