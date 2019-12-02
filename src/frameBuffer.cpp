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

void FrameBuffer::SetPixel(unsigned int x, unsigned int y, float newDepth, Color c)
{
    float currentDepth = depthBuffer.getDepth(x, y);
    if (!(x > 0 && x < width && y > 0 && y < height))
        return;

    Color oldColor = GetPixelColor(x, y);

    if (currentDepth > newDepth)
    {
    //     // Color newColor = c * (255.f/float(c.a)) + oldColor *  (255.f/float(oldColor.a)) * (1 - (255.f/float(c.a))) 
    //     //                 * (1 / (255.f / float(c.a) + 255.f / float(oldColor.a) * (1 - 255.f / float(c.a))));
        // if (c.a != 255)
        // {
            float div = (c.getTransparence() + oldColor.getTransparence() * (1 - c.getTransparence()));
            
            Color newColor = c * (c.getTransparence() / div) + oldColor * (oldColor.getTransparence() * (1 - c.getTransparence()) / div);
            newColor.a = (c.a + oldColor.a * (1 - c.a));
                            
            texture.SetPixelColor(x, y, newColor);
            depthBuffer.setDepth(x, y, newDepth);
        // }
        // else 
        // {
        //     texture.SetPixelColor(x, y, c);
        //     depthBuffer.setDepth(x, y, newDepth);
        // }
    }
    // else 
    //     std::cout << x << " / " << y << std::endl;
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
    
    // return;
    // for (unsigned int i = width*height-1; i--;)
    // {
    //     depthBuffer.depth[i] = depthMax;
    //     texture.pixels[i].r = 0;
    //     texture.pixels[i].g = 0;
    //     texture.pixels[i].b = 0;
    //     texture.pixels[i].a = 0;
    // }
    // return;

    // unsigned int size = width*height;
    // if (size % 2 != 0)
    // {
    //     depthBuffer.depth[0] = depthMax;
    //     texture.pixels[0] = Color(0,0,0);
    //     for (unsigned int i = 1; i < size; i+=2)
    //     {
    //         depthBuffer.depth[i] = depthMax;
    //         texture.pixels[i] = Color(0,0,0);
    //         depthBuffer.depth[i+1] = depthMax;
    //         texture.pixels[i+1] = Color(0,0,0);
    //     }
    // }
    // else 
    //     for (unsigned int i = 0; i < size; i+=2)
    //     {
    //         depthBuffer.depth[i] = depthMax;
    //         texture.pixels[i] = Color(0,0,0);
    //         depthBuffer.depth[i+1] = depthMax;
    //         texture.pixels[i+1] = Color(0,0,0);
    //     }
}
