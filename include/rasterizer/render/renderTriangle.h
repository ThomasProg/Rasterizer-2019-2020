#ifndef _RENDER_TRIANGLE_H_
#define _RENDER_TRIANGLE_H_

#include <array>
#include <vector>

#include "vec2.h"
#include "color.h"

#include "material.h"

class FrameBuffer;

class Vertex;
class RasterizingVertex;
//class Color;
class Light;
class Texture;

class RenderTriangle
{
// private:
//     //array of const Vertex& is not possible
//     //array of const pointers to const Vertex
//     std::array<RasterizingVertex const *, 3> triangleVertices;
//     FrameBuffer * const pTarget;
//     const std::vector<Light>& lights;
//     Texture* texture;

// private: 
//     float det;
//     float cst[4];

public:

    // RenderTriangle() = delete;

    // // RenderTriangle(std::array<Vertex const * const, 3> triangleVertices);
    // RenderTriangle(const std::array<RasterizingVertex, 3>& triangleVertices, FrameBuffer* pTarget, const std::vector<Light>& lights, Texture* texture);
    // //RenderTriangle(Vertex const * const v1, Vertex const * const v2, Vertex const * const v3);
    
    // // void draw();
    // // void fillTopFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices) const;
    // // void fillBottomFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices) const;
    // // void drawHorizontalLine(Vec2 p1, Vec2 p2, int height, const std::array<const Vertex*, 3>& triangleVertices) const;

    // //bool getWeight2D(const Vec2& p, float* weight) const;
    // //bool getWeight(const Vec2& p, float* weight) const;
    // Color getColorAtPixel(Vec2 p, bool& isValid, float* weight) const;

    // static float getPixelLight(const RasterizingVertex& vertex, const std::vector<Light>& lights, 
    //                             const Vec3& cameraLocation, const Material& mat);

    static float getPixelLight(const Vec3& location3D, 
                               const Vec3& normal, 
                               const std::vector<Light>& lights, 
                               const Vec3& cameraLocation, const Material& mat);
};

//#pragma region color interpolation

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

//#pragma endregion

// void drawTriangle(Vertex& vert1, Vertex& vert2, Vertex& vert3, Vec3 worldLoc1, Vec3 worldLoc2, Vec3 worldLoc3,
//                     float w1, float w2, float w3,
//                     const Vec3& cameraLocation,
//                     FrameBuffer* pTarget, std::vector<Light>& lights, const Material& mat, Texture* texture = nullptr);


// bool tryToDrawPixel(unsigned int x, unsigned int y, bool& isInside,
//                     const Vec3& v1, const Vec3& v2, const Vec3& v3,
//                     std::array<Vertex*, 3>& triangleVertices, 
//                     std::array<float, 3>&  ww, float* uP, float* vP, const Vec3& cameraLocation,
//                     std::array<Vec3, 3>& worldLoc, FrameBuffer* pTarget, std::vector<Light>& lights, 
//                     Texture* texture, const Material& mat, bool bShouldRecur = true);

#include "renderTriangle.inl"

#endif