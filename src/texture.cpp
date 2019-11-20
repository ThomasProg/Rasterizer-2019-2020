#include "texture.h"

#include "color.h"

Texture::Texture(unsigned int width, unsigned int height) 
    : width(width),
      height(height),
      pixels(new Color[width * height]) //default constructor init pixels to black
{

}

void Texture::SetPixelColor(unsigned int x, unsigned int y, Color c)
{
    pixels[x + y * width] = c;
}

Color Texture::GetPixelColor(unsigned int x, unsigned int y) const
{
    return pixels[x + y * width];   
}

void Texture::FillBlack()
{
    for (unsigned int i = 0; i < width * height; i++)
    {
        pixels[i].r = 0.f;
        pixels[i].g = 0.f;
        pixels[i].b = 0.f;
        pixels[i].a = 1.f;
    }
}

void Texture::getSize(unsigned int& width, unsigned int& height) const
{
    width  = this->width;
    height = this->height;
}