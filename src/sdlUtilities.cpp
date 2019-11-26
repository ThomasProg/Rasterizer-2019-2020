#include <iostream>
#include "sdlUtilities.h"

#include "frameBuffer.h"
#include "texture.h"
#include "color.h"
#include "macros.h"

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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

    isValid = true;
    return;
}

SDL_Utilities::~SDL_Utilities()
{
    std::cout << "SDL Error : " << SDL_GetError() << std::endl;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL_Utilities::SDL_RenderTexture(Texture& target)
{
    target.ToTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}