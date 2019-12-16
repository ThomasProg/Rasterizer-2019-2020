#ifndef _MACROS_H_
#define _MACROS_H_

#define __BACKFACE_CULLING__

#define __FIRST_PERSON__
//#define __THIRD_PERSON__

#define __PERSPECTIVE_FIX__

// #define __NEAREST_INTERPOLATION__
#define __BILINEAR_INTERPOLATION__


// #define __PHONG_LIGHTING__
#define __BLINN_PHONG_LIGHTING__

// #define __ENABLE_TRANSPARENCY__

//#define __ADDITIONAL_SHADERS__

//#define __SDL__
#define __GLFW__

constexpr float PI = 3.14159265359f;
constexpr float epsilon = 0.01;

constexpr float depthMax = 255;


// By enabling the __ANTI_ALIASING__, 
// there must be __ANTI_ALIASING__ or __MULTI_SAMPLING__ anti aliasing modes defined only.
// For the anti aliasing to be working, be sure to assign antiAliasingX or antiAliasingY to a value higher than 1.
// #define __ANTI_ALIASING__
// #define __SUPER_SAMPLING__
// #define __MULTI_SAMPLING_TRIANGLES__
// #define __MULTI_SAMPLING_LIGHT__
// #define __ANTI_ALIASING_DEBUG__  

constexpr unsigned int windowWidth  = 768;
constexpr unsigned int windowHeight = 768;

#ifdef __ANTI_ALIASING__
constexpr unsigned int antiAliasingX = 2;
constexpr unsigned int antiAliasingY = 2;
#else
constexpr unsigned int antiAliasingX = 1;
constexpr unsigned int antiAliasingY = 1;
#endif

constexpr float textureResolutionX = windowWidth  * antiAliasingX;
constexpr float textureResolutionY = windowHeight * antiAliasingY;


#define __ADD_OFFSET__

#ifdef __ADD_OFFSET__
constexpr float offset = 50.f;

constexpr float windowRenderMinX = offset * antiAliasingX;
constexpr float windowRenderMinY = offset * antiAliasingY;
constexpr float windowRenderMaxX = antiAliasingX * windowWidth  - offset * antiAliasingX;
constexpr float windowRenderMaxY = antiAliasingY * windowHeight - offset * antiAliasingY;
#endif

// ERRORS : 
// DO NOT EDIT
#if defined(__ANTI_ALIASING__) && defined(__SUPER_SAMPLING__) && defined(__MULTI_SAMPLING__)
#error Multiple anti aliasing modes running at the same time : undef __SUPER_SAMPLING__ or __MULTI_SAMPLING__
#endif

#if defined(__SDL__) && defined(__GLFW__)
#error __SDL__ and __GLFW__ both activated : there can be only one active.
#endif

#if defined(__NEAREST_INTERPOLATION__) && defined(__BILINEAR_INTERPOLATION__)
#error Both __NEAREST_INTERPOLATION__ and __BILINEAR_INTERPOLATION__ are activated : choose one!
#endif

#endif