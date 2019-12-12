#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

struct Color
{
    // union 
    // {
    //     struct 
    //     {
    //         unsigned char r, g, b, a;
    //     };
    //     struct 
    //     {
    //         unsigned char colors[4];
    //     };
    // };


    union 
    {
        struct 
        {
            float r, g, b, a;
        };
        struct 
        {
            float colors[4];
        };
    };

    // Color();
    // Color(unsigned char r, unsigned char g, unsigned char b);
    // Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);
    // Color(const Color& setColor);

    // //sizeof(Color) == 4, 
    // //so we use less memory copying by copy instead of reference (using 4 more bytes)
    // Color& operator=(Color setColor) noexcept;
    // Color& operator*=(float scalar) noexcept;
    // Color operator*(float scalar) noexcept;
    // Color operator+(const Color& rhs) noexcept;
    // Color operator/(unsigned char f) noexcept;
    // Color& operator+=(const Color& rhs) noexcept;
    // //unsigned char& operator[](unsigned int index) noexcept;

    Color();
    Color(float r, float g, float b);
    Color(float r, float g, float b, float a);
    Color(const Color& setColor);

    //sizeof(Color) == 4, 
    //so we use less memory copying by copy instead of reference (using 4 more bytes)
    Color& operator=(Color setColor) noexcept;
    Color& operator*=(float scalar) noexcept;
    Color operator*(float scalar) noexcept;
    Color operator+(const Color& rhs) noexcept;
    Color operator/(unsigned char f) noexcept;
    Color& operator+=(const Color& rhs) noexcept;
    //float& operator[](unsigned int index) noexcept;

    //void clamp();

    float getTransparence() const noexcept; //returns transparence 0 < a < 1
};

std::ostream& operator<<(std::ostream& stream, const Color& c);

Color getAverageColor(Color lhs, Color rhs, float lhsRatio) noexcept;

#endif