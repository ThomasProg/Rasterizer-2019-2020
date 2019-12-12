#include "renderTriangle2.h"

#include "vec4.h"
#include "mat4.h"

__inline
void RenderTriangle2::setupForTriangle(const Vertex& v1, const Vertex& v2, const Vertex& v3)
{
    this->v1 = v1;
    this->v2 = v2;
    this->v3 = v3;
}

// __inline
// void RenderTriangle2::setLocalToWorld(const Mat4& transform)
// {
//     v1.changeRef(transform);
//     v2.changeRef(transform);
//     v3.changeRef(transform);
// }

__inline
bool RenderTriangle2::isBackFaceCulled(const Vec3& cameraLocation)
{
    //is it really performant to compute normals for every triangle?
    //couldn't we do in another way, with already computed normals?

    //compute plane normal
    Vec3 N = crossProduct(Vec3((v2.position - v1.position)), Vec3((v3.position - v1.position)));

    worldVertices[0] = Vec4(v1.position, 1);
    worldVertices[1] = Vec4(v2.position, 1);
    worldVertices[2] = Vec4(v3.position, 1);

    return dotProduct(N, v1.position - cameraLocation) > 0;
}

__inline
void RenderTriangle2::setRelativeToCamera(const Mat4& transform)
{
    //TODO : optimization : Vec3 -> Vec4 -> Vec3
    v1.position = transform * Vec4(v1.position, 1);
    v2.position = transform * Vec4(v2.position, 1);
    v3.position = transform * Vec4(v3.position, 1);
}

__inline
std::array<float, 3> RenderTriangle2::projectVertices(const Mat4& projection)
{
    std::array<float, 3> w;

    //TODO: opti possible
    Vec4 projectedLoc = projection * v1.position;
    w[0] = projectedLoc.w;
    v1.position = projectedLoc.getHomogenizedVec();

    projectedLoc = projection * v2.position;
    w[1] = projectedLoc.w;
    v2.position = projectedLoc.getHomogenizedVec();

    projectedLoc = projection * v3.position;
    w[2] = projectedLoc.w;
    v3.position = projectedLoc.getHomogenizedVec();

    return w;
}

__inline
void RenderTriangle2::setVerticesToScreenResolution(const Mat4& screenConversion)
{
    v1.position = screenConversion * v1.position;
    v2.position = screenConversion * v2.position;
    v3.position = screenConversion * v3.position;
}

__inline
void RenderTriangle2::addTransparency(const float transparency)
{
    v1.color.a *= transparency;
    v2.color.a *= transparency;
    v3.color.a *= transparency;

    //std::cout << (unsigned int) (v1.color.a) << "  " << transparency << '\n';
}
#include "color.h"
__inline
void RenderTriangle2::setDefaultColor()
{
    v1.color = Color(1, 0, 0);
    v2.color = Color(0, 1, 0);
    v3.color = Color(0, 0, 1);
}

// #include "frameBuffer.h"
// #include "vec2.h"
// __inline
// void RenderTriangle2::drawLineX(FrameBuffer* pTarget, const Vertex& vertex1, const Vertex& vertex2)
// {
//     //get distance between two points
//     const int& x1 = vertex1.position.x;
//     const int& y1 = vertex1.position.y;
//     const int& x2 = vertex2.position.x;
//     const int& y2 = vertex2.position.y;
    
//     Vec2 vec2(x2-x1, y2-y1);
//     //get distance between the 2 points
//     float magnitude = vec2.GetMagnitude();

//     if (magnitude != 0.f)
//     {
//         vec2.toUnit();

//         for (float i = 0; i <= magnitude; ++i)
//         {
//             float ratio = i / magnitude;
//             Vec3 point(x1 + i * vec2.x, 
//                        y1 + i * vec2.y, 
//                        vertex1.position.z * ratio + vertex2.position.z * (1 - ratio));
//             //std::cout << "z : " << point.z << std::endl;

//             // float weight1 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
//             // float weight2 = Vec2(point.x - x1, point.y - y1).GetMagnitude() / magnitude;
            
//             //Color c(vertex1.color.r, vertex1.color.g, vertex1.color.b);
//             Color c(255,255,255);

//             pTarget->SetPixel(point.x, point.y, point.z, c);
//         }
//     }
//     else 
//         pTarget->SetPixel(x2, y2, vertex2.position.z, vertex1.color);
// }

__inline 
void RenderTriangle2::drawWireframe(FrameBuffer* pTarget)
{
//     drawLineX(pTarget, v1, v2);
//     drawLineX(pTarget, v2, v3);
//     drawLineX(pTarget, v3, v1);
}
#include "renderTriangle.h"
#include "light.h"
#include "texture.h"


__inline
void RenderTriangle2::drawTriangleX(FrameBuffer* pTarget, std::array<float, 3>& w, 
                                    const Vec3& cameraLocation, std::vector<Light>& lights, 
                                    Texture* pTexture, const Material& mat)
{
    // std::cout << w[0] << '\n';
    // std::cout << w[1] << '\n';
    // std::cout << w[2] << '\n';
    drawTriangle(v1, v2, v3, 
                    worldVertices[0], worldVertices[1], worldVertices[2], 
                    w[0], w[1], w[2],
                    cameraLocation,
                    pTarget, lights, mat, pTexture);
}