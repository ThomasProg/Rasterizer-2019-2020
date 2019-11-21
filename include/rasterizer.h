#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <vector>

class Scene;
class Texture;
class Entity;
class Vertex;
class Vertex2;

class Rasterizer
{
private:

public:
    static void RenderPoints(Texture* pTarget, 
                             const std::vector<Vertex2>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderLines(Texture* pTarget, 
                             const std::vector<Vertex2>& vertices, std::vector<unsigned int>::iterator& it);

    // static void RenderTriangles(Texture* pTarget, 
    //                          const std::vector<Vertex2>& vertices, std::vector<unsigned int>::iterator& it);

    static void ClipTriangles(const Entity& entity, Texture* pTarget, 
                                const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it,
                                //outputs :
                                std::vector<Vertex2>& transformedVertices, 
                                std::vector<unsigned int>& transformedIndices);
    static void ClipLines(const Entity& entity, Texture* pTarget, 
                           const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                           //outputs :
                           std::vector<Vertex2>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices);

    static void RenderScene(Scene* pScene, Texture* pTarget);
};

#endif