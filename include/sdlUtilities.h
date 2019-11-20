#ifndef _SDL_UTILITIES_H_
#define _SDL_UTILITIES_H_

#include <SDL2/SDL.h>
class Texture;

struct SDL_Utilities
{
public:
    SDL_Window* window;
    SDL_Renderer* renderer;
public:
    SDL_Utilities(bool& isValid);
    ~SDL_Utilities();

    static void textureToSDL(const Texture& tex, SDL_Renderer* renderer);
};

#endif