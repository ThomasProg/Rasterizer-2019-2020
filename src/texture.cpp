#include <cassert>
// #include <SDL2/SDL.h>
// #include <SDL2/SDL_image.h>
#include <iostream>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "texture.h"

#include "color.h"

Texture::Texture(const Texture& rhs)
    : width(rhs.width), height(rhs.height), pixels(new Color[width * height])
{
    pixels = new Color[width * height];
    for (unsigned int i = 0; i < width * height; i++)
    {
        pixels[i] = rhs.pixels[i];
    }
}

Texture::Texture(Texture&& rhs) noexcept
    : width(std::move(rhs.width)),
      height(std::move(rhs.height)),
      pixels(std::move(rhs.pixels))
{
    rhs.pixels = nullptr;
}

Texture::Texture(const char* filename)
    : width(0),
      height(0),
      pixels() //default constructor init pixels to black
{
    int h, w, comp;
    unsigned char* image = stbi_load(filename, &w, &h, &comp, STBI_rgb_alpha);

    if (image != NULL)
    {
        width = w;
        height = h;

        pixels = new Color[width * height];

        for (unsigned int y = 0; y < height; y++)
        { 
            for (unsigned int x = 0; x < width; x++)
            { 
                for (int i = 0; i < comp; i++)
                {
                    pixels[y * width + x].colors[i] = image[comp * (y * width + x) + i] / 255.f;
                }
            }
        }

        stbi_image_free(image);
    }
}

void Texture::setDegradee()
{
    for (unsigned int i = 0; i < height; i++)
    {
        for (unsigned int j = 0; j < width; j++)
        {
            pixels[j + i * width].r = 255.f / height * i;
            pixels[j + i * width].g = 0;
            pixels[j + i * width].b = 0;
            pixels[j + i * width].a = 255;
        }
    }
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
        delete[] pixels;
}

void Texture::FillBlack()
{
    for (unsigned int i = 0; i < width * height; i++)
    {
        pixels[i] = {0, 0, 0, 1};
    }
}
