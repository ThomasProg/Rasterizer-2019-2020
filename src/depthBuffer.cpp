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
