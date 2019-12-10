#ifndef _MACROS_H_
#define _MACROS_H_

#define __BACKFACE_CULLING__
#define __FIRST_PERSON__

#define __PERSPECTIVE_FIX__

// #define __NEAREST_INTERPOLATION__
#define __BILINEAR_INTERPOLATION__

#define __ANTI_ALIASING__

//#define __SDL__
#define __GLFW__

#define __FORCE_UNTEXTURED_TRIANGLE_COLOR__ \
        vert1.color = Color(250,250,250);   \
        vert2.color = Color(250,100,250);   \
        vert3.color = Color(250,250,100);

constexpr float PI = 3.14159265359f;
constexpr float epsilon = 0.01;

constexpr unsigned int windowWidth  = 768;
constexpr unsigned int windowHeight = 768;

constexpr float depthMax = 255;

constexpr unsigned int antiAliasingX = 2;
constexpr unsigned int antiAliasingY = 2;

constexpr float textureResolutionX = windowWidth  * antiAliasingX;
constexpr float textureResolutionY = windowHeight * antiAliasingY;

#endif