#ifndef _RENDER_TRIANGLE_2_H_
#define _RENDER_TRIANGLE_2_H_

#include <array>
#include <vector>

#include "vertex.h"
#include "vec4.h"

#include "material.h"

class FrameBuffer;
class Light;
class Texture;

class RenderTriangle2
{
//private?
public:
    std::array<Vertex, 3> triangleVertices;
    // Vertex v1, v2, v3;

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
    void setRelativeToCamera(const Mat4& transform);

    __inline
    bool isClipped(const Texture* pTarget);

    __inline
    std::array<float, 3> projectVertices(const Mat4& projection);

    __inline
    void setVerticesToScreenResolution(const Mat4& screenConversion);

    __inline
    void addTransparency(const float transparency);

    __inline
    void setDefaultColor();

    __inline
    static void drawLineX(FrameBuffer* pTarget, const Vertex& vertex1, const Vertex& vertex2);

    __inline 
    void drawWireframe(FrameBuffer* pTarget);

    __inline
    void drawTriangleX(FrameBuffer* pTarget, std::array<float, 3>& ww, const Vec3& cameraLocation, 
                        std::vector<Light>& lights, Texture* pTexture, const Material& mat);
};

#include "renderTriangle2.inl"

#endif