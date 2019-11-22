#ifndef _TEXTURE_H_
#define _TEXTURE_H_

struct Color;

class Texture
{
public:
    const unsigned int width, height;
private:
    Color* pixels = nullptr;

public:
    Texture() = delete;
    Texture(unsigned int width, unsigned int height);
    ~Texture();

    void SetPixelColor(unsigned int x, unsigned int y, Color c);
    Color GetPixelColor(unsigned int x, unsigned int y) const;

    void FillBlack();
};

#endif