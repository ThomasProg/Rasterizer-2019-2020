#include <cassert>

#include "texture.h"

__inline
void Texture::SetPixelColor(unsigned int x, unsigned int y, const Color& c)
{
    if (x > 0 && x < width && y > 0 && y < height)
        pixels[x + y * width] = c;
}

__inline
Color Texture::GetPixelColor(unsigned int x, unsigned int y) const
{
    // x <= 0 && y <= 0 since they are unsigned
    assert(x < width && y < height);
    return pixels[x + y * width];   
}