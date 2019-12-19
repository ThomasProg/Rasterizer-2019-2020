#include "depthBuffer.h"

__inline
float DepthBuffer::getDepth(unsigned int x, unsigned int y) const
{
    // x >= 0 && y >= 0 since they are unsigned
    if (x < width && y < height)
        return depth[x + y * width];   
    else 
        return -1;
}

__inline
void DepthBuffer::setDepth(unsigned int x, unsigned int y, float newDepth)
{
    // x >= 0 && y >= 0 since they are unsigned
    if (x < width && y < height)
        depth[x + y * width] = newDepth;
}