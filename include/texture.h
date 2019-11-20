#ifndef _TEXTURE_H_
#define _TEXTURE_H_

struct Color;

class Texture
{
private:
    unsigned int width, height;
    Color* pixels = nullptr;

public:
    Texture() = delete;
    Texture(unsigned int width, unsigned int height);

    void SetPixelColor(unsigned int x, unsigned int y, Color c);
    Color GetPixelColor(unsigned int x, unsigned int y) const;

    void FillBlack();

    //
    void getSize(unsigned int& width, unsigned int& height) const;
};

#endif