#include "frameBuffer.h"

__inline
void FrameBuffer::SetPixel(unsigned int x, unsigned int y, float newDepth, const Color& c)
{
    assert(x >= 0 && x < width && y >= 0 && y < height);

    #ifdef __ENABLE_TRANSPARENCY__
    if (c.a >= 1.f)
    {
        texture.SetPixelColor(x, y, c);
    }
    else 
    {
        Color oldColor = GetPixelColor(x, y);
        float div = (c.getTransparence() + oldColor.getTransparence() * (1 - c.getTransparence()));

        Color newColor = c * (c.getTransparence() / div) + oldColor * (oldColor.getTransparence() * (1 - c.getTransparence()) / div);
        newColor.a = (c.a + oldColor.a * (1 - c.a));
        texture.SetPixelColor(x, y, newColor);
    }
    #else
    texture.SetPixelColor(x, y, c);
    #endif

    depthBuffer.setDepth(x, y, newDepth);
}

__inline
Color FrameBuffer::GetPixelColor(unsigned int x, unsigned int y) const
{
    return texture.GetPixelColor(x, y);   
}

__inline
float FrameBuffer::GetDepth(unsigned int x, unsigned int y) const
{
    return depthBuffer.getDepth(x, y);
}