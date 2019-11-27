#include <cassert>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "texture.h"

#include "color.h"

Texture::Texture(const char* filename)
    : width(width),
      height(height),
      pixels(new Color[width * height]) //default constructor init pixels to black
{
    // SDL_Surface* surface = IMG_Load(filename);
    // pixels = surface->pixels;
    // SDL_FreeSurface(surface);
}

Texture::Texture(unsigned int width, unsigned int height) 
    : width(width),
      height(height),
      pixels(new Color[width * height]) //default constructor init pixels to black
{
    
}

Texture::~Texture()
{
    if (pixels != nullptr)
        delete pixels;
}

void Texture::SetPixelColor(unsigned int x, unsigned int y, Color c)
{
    if (x > 0 && x < width && y > 0 && y < height)
        pixels[x + y * width] = c;
}

Color Texture::GetPixelColor(unsigned int x, unsigned int y) const
{
    assert(0 <= x && x < width && 0 <= y && y < height);
    return pixels[x + y * width];   
}

void Texture::FillBlack()
{
    // 4 * sizeof(unsigned char)
    memset(pixels, 0, 4 * sizeof(unsigned char) * width * height);
    // for (unsigned int i = 0; i < width * height; i++)
    // {
    //     pixels[i].r = 0.f;
    //     pixels[i].g = 0.f;
    //     pixels[i].b = 0.f;
    //     pixels[i].a = 1.f;
    // }
}

void Texture::ToTexture(SDL_Texture* sdlTexture) const
{
    SDL_UpdateTexture(sdlTexture, nullptr, pixels, sizeof(Color) * width); 
}