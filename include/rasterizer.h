#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <vector>

class Scene;
class Texture;
class FrameBuffer;

class Entity;
class Vertex;
class Vertex2;

class Rasterizer
{
private:

public:
    static void RenderPoints(FrameBuffer* pTarget, 
                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderLines(FrameBuffer* pTarget, 
                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderTriangles(FrameBuffer* pTarget, 
                                const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

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

    static void RenderScene(Scene* pScene, FrameBuffer* pTarget);
};

#endif