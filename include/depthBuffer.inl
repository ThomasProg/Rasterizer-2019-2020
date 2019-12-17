#include "depthBuffer.h"

__inline
float DepthBuffer::getDepth(unsigned int x, unsigned int y) const
{
    //assert(0 <= x && x < width && 0 <= y && y < height);
    if (0 <= x && x < width && 0 <= y && y < height)
        return depth[x + y * width];   
    else 
        return -1;
}

__inline
void DepthBuffer::setDepth(unsigned int x, unsigned int y, float newDepth)
{
    if (0 <= x && x < width && 0 <= y && y < height)
        depth[x + y * width] = newDepth;
}