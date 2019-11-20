#include <iostream>
#include "sdlUtilities.h"

#include "texture.h"
#include "color.h"

constexpr unsigned int windowWidth = 1024;
constexpr unsigned int windowHeight = 768;

//sdl class
//returns true if successfuly init sdl
SDL_Utilities::SDL_Utilities(bool& isValid)
{
    //init SDL
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        isValid = false;
        return;
    }

    window   = SDL_CreateWindow("window", 0, 0, windowWidth, windowHeight, SDL_WINDOW_RESIZABLE);
    renderer = SDL_CreateRenderer(window, 0, SDL_RENDERER_ACCELERATED);

    // SDL_Texture* texture = SDL_CreateTexture(renderer,
    //     SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STATIC, windowWidth, windowHeight);
    // static Color pixels[windowWidth * windowHeight];
    // memset(pixels, 255, windowWidth * windowHeight * sizeof(char)*4);

    isValid = true;
    return;
}

SDL_Utilities::~SDL_Utilities()
{
    std::cout << "SDL Error : " << SDL_GetError() << std::endl;
    SDL_DestroyWindow(window);
    SDL_DestroyRenderer(renderer);
    SDL_Quit();
}

void SDL_Utilities::textureToSDL(const Texture& tex, SDL_Renderer* renderer)
{
    unsigned int width, height;
    tex.getSize(width, height);


    Color color;
    for (unsigned int y = 0; y < height; y++)
    {
        for (unsigned int x = 0; x < width; x++)
        {
            color = tex.GetPixelColor(x, y);
            // if (x % 15 == 0 && y % 15 == 0)
            // color.r = 255;

            // if ((x+y+3) % 15 == 0)
            // color.b = 255;
            // if ((x+y+9) % 15 == 0)
            // color.g = 255;
            SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
            SDL_RenderDrawPoint(renderer, x, y);
        }
    }
}
