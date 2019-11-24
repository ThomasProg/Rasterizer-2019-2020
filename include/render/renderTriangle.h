#ifndef _RENDER_TRIANGLE_H_
#define _RENDER_TRIANGLE_H_

#include <array>

#include "vec2.h"

class FrameBuffer;

class Vertex;
class Color;

class RenderTriangle
{
private:
    //array of const Vertex& is not possible
    //array of const pointers to const Vertex
    std::array<Vertex const *, 3> triangleVertices;
    FrameBuffer * const pTarget;

private: 
    float det;
    float cst[4];

public:

    RenderTriangle() = delete;

    // RenderTriangle(std::array<Vertex const * const, 3> triangleVertices);
    RenderTriangle(const std::array<const Vertex*, 3>& triangleVertices, FrameBuffer* pTarget);
    //RenderTriangle(Vertex const * const v1, Vertex const * const v2, Vertex const * const v3);
    
    void draw();
    void fillTopFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices) const;
    void fillBottomFlatTriangle(const std::array<const Vertex*, 3>& triangleVertices) const;
    void drawHorizontalLine(Vec2 p1, Vec2 p2, int height, const std::array<const Vertex*, 3>& triangleVertices) const;

    bool getWeight(const Vec2& p, float* weight) const;
    Color getColorAtPixel(Vec2 p, bool& isValid, float* weight) const;
};

#endif