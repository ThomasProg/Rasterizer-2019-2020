#ifndef _SDL_UTILITIES_H_
#define _SDL_UTILITIES_H_

#include <SDL2/SDL.h>

class Texture;

struct SDL_Utilities
{
public:
    SDL_Window*   window;
    SDL_Renderer* renderer;
    SDL_Texture*  texture;

public:
    SDL_Utilities(bool& isValid);
    ~SDL_Utilities();

    void SDL_RenderTexture(Texture& target);
};

#endif