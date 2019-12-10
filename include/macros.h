#ifndef _MACROS_H_
#define _MACROS_H_

#define __BACKFACE_CULLING__
#define __FIRST_PERSON__

#define __NEAREST_INTERPOLATION__
#define __BILINEAR_INTERPOLATION__

#define __FORCE_UNTEXTURED_TRIANGLE_COLOR__ \
        vert1.color = Color(250,250,250);   \
        vert2.color = Color(250,100,250);   \
        vert3.color = Color(250,250,100);

constexpr float PI = 3.14159265359f;
constexpr float epsilon = 0.01;

constexpr unsigned int windowWidth  = 768;
constexpr unsigned int windowHeight = 768;

constexpr float depthMax = 255;

#endif