#ifndef _COLOR_H_
#define _COLOR_H_

#include <iostream>

struct Color
{
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

    Color();
    __inline
    Color(float r, float g, float b);
    __inline
    Color(float r, float g, float b, float a);
    __inline
    Color(const Color& setColor);

    //sizeof(Color) == 4, 
    //so we use less memory copying by copy instead of reference (using 4 more bytes)
    Color& operator=(const Color& setColor) noexcept;
    __inline
    Color& operator*=(float scalar) noexcept;
    __inline
    Color operator*(float scalar) const noexcept;
    __inline
    Color operator+(const Color& rhs) const noexcept;
    __inline
    Color operator/(unsigned char f) const noexcept;
    __inline
    Color& operator+=(const Color& rhs) noexcept;
    //float& operator[](unsigned int index) noexcept;

    //void clamp();

    __inline
    float getTransparence() const noexcept; //returns transparence 0 < a < 1
    __inline
    void  copyRGB(const Color& rhs) noexcept;
    __inline
    void reset() noexcept;
};

std::ostream& operator<<(std::ostream& stream, const Color& c);

__inline
Color getAverageColor(const Color& lhs, const Color& rhs, float lhsRatio) noexcept;

#include "color.inl"

#endif