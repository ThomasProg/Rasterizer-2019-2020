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
class Light;

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

    static void RenderPoints(FrameBuffer* pTarget, std::vector<Light>& lights,
                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderLines(FrameBuffer* pTarget, std::vector<Light>& lights,
                             std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderTriangles(FrameBuffer* pTarget, std::vector<Light>& lights,
                                std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderWireframe(FrameBuffer* pTarget, std::vector<Light>& lights,
                                std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);


    static void ClipTriangles(const Entity& entity, FrameBuffer* pTarget, 
                                const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it,
                                //outputs :
                                std::vector<Vertex>& transformedVertices, 
                                std::vector<unsigned int>& transformedIndices);
    static void ClipLines(const Entity& entity, FrameBuffer* pTarget, 
                           const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                           //outputs :
                           std::vector<Vertex>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices);

    static void ClipPoints(const Entity& entity, FrameBuffer* pTarget, 
                           const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                           //outputs :
                           std::vector<Vertex>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices);

    static void RenderScene(Scene* pScene, FrameBuffer* pTarget, E_RasterizerMode mode = E_RasterizerMode::E_POINTS);
};

#endif