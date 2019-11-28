#ifndef _TEXTURE_H_
#define _TEXTURE_H_

struct SDL_Texture;
struct Color;

class Texture
{
public:
    unsigned int width, height; //TODO : since can't const, make accessor
public:
    Color* pixels = nullptr;

public:
    Texture() = delete;
    Texture(const char* filename);
    Texture(unsigned int width, unsigned int height);
    ~Texture();

    void SetPixelColor(unsigned int x, unsigned int y, Color c);
    Color GetPixelColor(unsigned int x, unsigned int y) const;

    void FillBlack();

    void ToTexture(SDL_Texture* sdlTexture) const;

    void setDegradee();
};

#endif