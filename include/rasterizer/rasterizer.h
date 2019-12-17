#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <vector>

class Scene;
class Texture;
class FrameBuffer;

class Entity;
class Vertex;
class Vertex2;
class Vec3;
class Mat4;
class Light;

class Camera;

enum class E_RasterizerMode
{
    E_TRIANGLES,
    E_LINES,
    E_POINTS,

    E_WIREFRAME, 
    E_TRIANGLES_AS_LINES, //same as wireframe
};

class Rasterizer
{
private:

public:
    static void antiAliasingCompression(const FrameBuffer& highResolutionTexture, Texture& finalTexture);

    static void RenderScene(Scene* pScene, FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix,  Camera& camera,
                            E_RasterizerMode mode = E_RasterizerMode::E_POINTS);
};

#endif