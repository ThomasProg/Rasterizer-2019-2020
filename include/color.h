#ifndef _COLOR_H_
#define _COLOR_H_

struct Color
{
    unsigned char r, g, b, a;

    Color();
    Color(unsigned char r, unsigned char g, unsigned char b);
    Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    Color(const Color& setColor);

    //sizeof(Color) == 4, 
    //so we use less memory copying by copy instead of reference (using 4 more bytes)
    Color& operator=(Color setColor);
    Color& operator*=(float scalar);
    unsigned char& operator[](unsigned int index);
};

#endif