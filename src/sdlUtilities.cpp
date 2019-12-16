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

    texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_STREAMING, windowWidth, windowHeight);

    isValid = true;
    return;
}

SDL_Utilities::~SDL_Utilities()
{
    std::cout << "SDL Success : " << SDL_GetError() << std::endl;
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void SDL_Utilities::SDL_RenderTexture(Texture& target)
{
    // Texture renderedTarget(windowWidth, windowWidth);
    
    // for (unsigned int y = 0; y < renderedTarget.height; y++)
    // {
    //     for (unsigned int x = 0; x < renderedTarget.width; x++)
    //     {
    //         //unsigned int id = x * antiAliasingX + y * (target.width) * antiAliasingY;

    //         Color finalColor (0, 0, 0, 0);

    //         // //get pixel color
    //         // for (unsigned int i = 0; i < antiAliasingY; i++)
    //         // {
    //         //     for (unsigned int j = 0; j < antiAliasingX; j++)
    //         //     {       
    //         //         finalColor += target.pixels[((x * antiAliasingX + j) 
    //         //                     + (y * antiAliasingY * renderedTarget.width + i))
    //         //                     / (antiAliasingX * antiAliasingY)];
    //         //     }  
    //         // }

    //         //finalColor = target.pixels[x * 4 + y * renderedTarget.width * 2 * 2];

    //         //renderedTarget.pixels[x + y * renderedTarget.width] = std::move(finalColor);
    //         renderedTarget.pixels[x + y * renderedTarget.width] = std::move(target.pixels[x + y * renderedTarget.width]);
    //     }
    // }

    target.ToTexture(texture);

    SDL_RenderClear(renderer);
    SDL_RenderCopy(renderer, texture, nullptr, nullptr);
    SDL_RenderPresent(renderer);
}