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
    static Vec3 projection(const Vec3& vec);

    // static void RenderPoints(FrameBuffer* pTarget, std::vector<Light>& lights,
    //                          std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);
    static void RenderPoints(FrameBuffer* pTarget, 
                                const std::vector<Light>& lights,
                                const std::vector<Vec3>&  screenVertices, 
                                const std::vector<Vertex>& worldVertices, 
                                Texture* texture);

    static void RenderLines(FrameBuffer* pTarget, std::vector<Light>& lights,
                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    // static void RenderTriangles(FrameBuffer* pTarget, std::vector<Light>& lights,
    //                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderTriangles(FrameBuffer* pTarget, 
                                const std::vector<Light>& lights,
                                const std::vector<Vec3>&  screenVertices, 
                                const std::vector<Vertex>& worldVertices,
                                Texture* texture);

    // static void RenderWireframe(FrameBuffer* pTarget, std::vector<Light>& lights,
    //                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);
    static void RenderWireframe(FrameBuffer* pTarget, 
                                const std::vector<Light>& lights,
                                const std::vector<Vec3>&  screenVertices, 
                                const std::vector<Vertex>& worldVertices,
                                Texture* texture);


    // static void ClipTriangles(const Entity& entity, FrameBuffer* pTarget, 
    //                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it,
    //                             //outputs :
    //                             std::vector<Vertex>& transformedVertices, 
    //                             std::vector<unsigned int>& transformedIndices);
    // static void ClipLines(const Entity& entity, FrameBuffer* pTarget, 
    //                        const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
    //                        //outputs :
    //                        std::vector<Vertex>& transformedVertices, 
    //                        std::vector<unsigned int>& transformedIndices);

    // static void ClipPoints(const Entity& entity, FrameBuffer* pTarget, 
    //                        const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
    //                        //outputs :
    //                        std::vector<Vertex>& transformedVertices, 
    //                        std::vector<unsigned int>& transformedIndices);

    static void antiAliasingCompression(const Texture& highResolutionTexture, Texture& finalTexture);

    static void RenderScene(Scene* pScene, FrameBuffer* pTarget, const Mat4& projectionMatrix, const Mat4& inverseCameraMatrix,  Camera& camera,
                            E_RasterizerMode mode = E_RasterizerMode::E_POINTS);
};

#endif