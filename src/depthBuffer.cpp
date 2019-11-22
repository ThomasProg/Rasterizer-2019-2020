#include <cassert>
#include "depthBuffer.h"


DepthBuffer::DepthBuffer(unsigned int width, unsigned int height) 
    : width(width),
      height(height),
      depth(new float[width * height])
{
    
}

DepthBuffer::~DepthBuffer()
{
    if (depth != nullptr)
        delete depth;
}

void DepthBuffer::resetDepth()
{
    for (unsigned int i = 0; i < width * height; i++)
        depth[i] = 100;
}

float DepthBuffer::getDepth(unsigned int x, unsigned int y) const
{
    assert(0 <= x && x < width && 0 <= y && y < height);
    return depth[x + y * width];   
}

void DepthBuffer::setDepth(unsigned int x, unsigned int y, float newDepth)
{
    assert(0 <= x && x < width && 0 <= y && y < height);
    depth[x + y * width] = newDepth;
}