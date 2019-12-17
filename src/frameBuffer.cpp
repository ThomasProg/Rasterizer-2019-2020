#include <cassert>
#include "frameBuffer.h"
#include "color.h"

#include "macros.h"

FrameBuffer::~FrameBuffer()
{

}

FrameBuffer::FrameBuffer(unsigned int width, unsigned int height) 
    : width(width), 
      height(height), 
      texture(width, height), 
      depthBuffer(width, height)
{
    
}

void FrameBuffer::ResetPixels()
{
    texture.FillBlack();
    depthBuffer.resetDepth();
}
