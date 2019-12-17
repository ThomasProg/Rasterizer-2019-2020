#ifndef _TEXTURE_H_
#define _TEXTURE_H_

struct SDL_Texture;

#include "color.h"

class Texture
{
public:
    unsigned int width = 0, height = 0; //TODO : since can't const, make accessor
public:
    Color* pixels = nullptr;

public:
    Texture() = delete;
    Texture(const Texture& rhs);
    Texture(Texture&& rhs) noexcept;
    Texture(const char* filename);
    Texture(unsigned int width, unsigned int height);
    ~Texture();


    __inline
    void SetPixelColor(unsigned int x, unsigned int y, const Color& c);
    __inline
    Color GetPixelColor(unsigned int x, unsigned int y) const;

    void FillBlack();

    // void ToTexture(SDL_Texture* sdlTexture) const;

    void setDegradee();
};

#include "texture.inl"

#endif