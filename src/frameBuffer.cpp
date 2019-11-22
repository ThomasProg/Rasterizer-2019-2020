#include "frameBuffer.h"
#include "color.h"

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

void FrameBuffer::SetPixel(unsigned int x, unsigned int y, float newDepth, Color c)
{
    float currentDepth = depthBuffer.getDepth(x, y);
    if (currentDepth > newDepth)
    {
        texture.SetPixelColor(x, y, c);
        depthBuffer.setDepth(x, y, newDepth);
    }
}

Color FrameBuffer::GetPixelColor(unsigned int x, unsigned int y) const
{
    return texture.GetPixelColor(x, y);   
}

float FrameBuffer::GetDepth(unsigned int x, unsigned int y) const
{
    return depthBuffer.getDepth(x, y);
}

void FrameBuffer::ResetPixels()
{
    texture.FillBlack();
    depthBuffer.resetDepth();
}
