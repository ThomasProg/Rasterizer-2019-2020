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
    bool isClipped(const Texture& pTarget, 
                   std::vector<RenderTriangle2>& additionalTriangles);

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


    void projectAndDraw(std::vector<Light>& lights, const Entity* entity,
                        FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& screenConversionMatrix, 
                        Camera& camera, E_RasterizerMode mode);
};

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

#include "renderTriangle2.inl"

#endif