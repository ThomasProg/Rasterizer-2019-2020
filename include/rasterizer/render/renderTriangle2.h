#ifndef _RENDER_TRIANGLE_2_H_
#define _RENDER_TRIANGLE_2_H_

#include <array>
#include <vector>

#include "vertex.h"
#include "vec4.h"

#include "material.h"
#include "rasterizer.h"

class FrameBuffer;
class Light;
class Texture;
class Entity;
class Camera;
struct Vec2;

// Used to store the triangle constants for the weight 
struct weightPrecomputedData
{
    inline
    weightPrecomputedData(const float p2p3y, const float p1p3y, const float p3p2x, const float p1p3x) 
        : p2p3y(p2p3y), p1p3y(p1p3y), p3p2x(p3p2x), p1p3x(p1p3x), det(p2p3y * p1p3x + p3p2x * p1p3y)
        {}

    float p2p3y; // p2.y - p3.y
    float p1p3y; // p3.y - p1.y
    float p3p2x; // p3.x - p2.x
    float p1p3x; // p1.x - p3.x

    float det;
};

struct RenderTriangle2
{
public:
    std::array<Vertex, 3> triangleVertices;

    std::array<Vec3, 3> worldVertices;

public:
    RenderTriangle2() {}

    __inline
    void setupForTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3);

    // __inline
    // void setLocalToWorld(const Mat4& transform);

    __inline
    bool isBackFaceCulled(const Vec3& cameraLocation);

    __inline
    bool isClipped(const Texture& pTarget, 
                   std::vector<RenderTriangle2>& additionalTriangles, 
                   std::array<Vec4, 3>& projectedTriangles);

    __inline
    void setVerticesToScreenResolution(const Mat4& screenConversion);

    __inline
    void addTransparency(const float transparency);

    __inline
    static void drawLineX(FrameBuffer* pTarget, const Vertex& vertex1, const Vertex& vertex2);

    __inline 
    void drawWireframe(FrameBuffer* pTarget);

    __inline
    void drawTriangleX(FrameBuffer* pTarget, std::array<float, 3>& ww, const Vec3& cameraLocation, 
                        std::vector<Light>& lights, Texture* pTexture, const Material& mat);


    void projectAndDraw(std::vector<Light>& lights, const Entity* entity,
                        FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& screenConversionMatrix, 
                        Camera& camera, E_RasterizerMode mode, std::array<float, 3>& w);
};

// Default Version : > 25 operations
// calcule the weight of a point (p) in a triangle of vertices p1, p2, p3.
inline
void getWeight(const Vec2& p, const Vec3& p1, const Vec3& p2, const Vec3& p3, 
                std::array<float, 3>& weight);

// 12 operations : see the other declaration of getWeight for more details
inline
void getWeight(const float pxp3x, const float pyp3y, 
                std::array<float, 3>& weight, 
                const weightPrecomputedData& precomputed);

__inline
void getUntexturedPixelColor(const std::array<Vertex, 3>& triangleVertices, 
                            const std::array<float, 3>& weight, 
                            Color& colorOutput);

__inline
void getTexturedColorNearestInterpolation(const Texture*, 
                                          const float u,
                                          const float v, 
                                          Color& colorOutput);

__inline
void getTexturedColorBilinearInterpolation(const Texture*, 
                                           const float u,
                                           const float v, 
                                           Color& colorOutput);

__inline
float getPixelLight(const Vec3& location3D, 
                    const Vec3& normal, 
                    const std::vector<Light>& lights, 
                    const Vec3& cameraLocation, const Material& mat);


// Clipping Functions for triangles
namespace triangles
{
    __inline
    void clipHorizontal(float& x1, float& y1, const Vec2& vec2, float limit);
    __inline
    void clipVertical(float& x1, float& y1, const Vec2& vec2, float limit);
};

// Clipping Functions for lines
// order is important because of the slope :
// that is why there are "OtherPoint" functions

// x1 and y1 are the coordinates of the first point,
// x2 and y2 of the second point,
// vec2 is the slope,
// magnitude is the distance between the 2 points, which will be modified,
// limit is the limit on x or y 
namespace lines
{
    __inline
    void clipHorizontal(int& x1, int& y1, int x2, int y2, 
                        Vec2& vec2, float& magnitude, float limit);

    __inline
    void clipHorizontalOtherPoint(int x1, int y1, int& x2, int& y2, 
                                  Vec2& vec2, float& magnitude, float limit);

    __inline
    void clipVertical(int& x1, int& y1, int x2, int y2, 
                      Vec2& vec2, float& magnitude, float limit);

    __inline
    void clipVerticalOtherPoint(int x1, int y1, int& x2, int& y2, 
                                Vec2& vec2, float& magnitude, float limit);
};

// Try to put into the framebuffer the Color (given by getColor()) and the depth 
// if possible.
// getColor() is called after the depthBuffer test, and so optimization can occur.
// The lambda function parameter also allows to calls this function whenever you want
// to draw a pixel relative to the depth.
bool tryToDrawPixel(unsigned int x, unsigned int y, float depth, 
                    FrameBuffer* pTarget, std::function<Color(void)>& getColor);


#include "renderTriangle2.inl"

#endif