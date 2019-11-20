#ifndef _RASTERIZER_H_
#define _RASTERIZER_H_

#include <vector>

class Scene;
class Texture;
class Entity;
class Vertex;

class Rasterizer
{
private:

public:
    static void RenderPoints(const Entity& entity, Texture* pTarget, 
                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderLines(const Entity& entity, Texture* pTarget, 
                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderTriangles(const Entity& entity, Texture* pTarget, 
                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it);

    static void RenderScene(Scene* pScene, Texture* pTarget);
};

#endif