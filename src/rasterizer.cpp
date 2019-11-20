#include <cassert>
#include <iostream>

#include "rasterizer.h"

#include "macros.h"

#include "scene.h"
#include "texture.h"
#include "entity.h"
#include "vertex.h"
#include "mesh.h"
#include "vec2.h"
#include "vec3.h"
#include "vec4.h"
#include "color.h"

typedef std::vector<unsigned int>::iterator indiceIt;

void projection(Vec3 vec, int& x, int& y)
{
    x = (vec.x/2.f + 0.5f) * windowWidth;
    y = (vec.y/2.f + 0.5f) * windowHeight;

    // x = ((vec.x / 5) + 1) * 0.5 * windowWidth;
    // y = ((vec.y / 5) + 1) * 0.5 * windowHeight;
}

void convertVertexToWindow(const Mat4& transformation, Vec3 position, int& x, int& y)
{
    Vec4 newLoc = transformation * Vec4(position, 1);
    newLoc.Homogenize();

    Vec3 newLoc3D(newLoc.x, newLoc.y, newLoc.z);
    projection(newLoc3D, x, y);
}

/* 
 * if used, draw vertices as points
 * Complexity : O(1)
 */
void Rasterizer::RenderPoints(const Entity& entity, Texture* pTarget, 
                              const std::vector<Vertex>& vertices, indiceIt& it)
{
    const Vertex& vertex = entity.mesh->vertices[*it++];

    int x, y;
    convertVertexToWindow(entity.transformation, vertex.position, x, y);

    Color c(0,0,255,0);

    pTarget->SetPixelColor(x, y, c);
}

/* 
 * if used, draw vertices as lines
 * Complexity : O(n), n being the distance on screen between the 2 points
 */
void Rasterizer::RenderLines(const Entity& entity, Texture* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    //get first point on screen
    const Vertex& vertex1 = entity.mesh->vertices[*it++];

    int x1, y1;
    convertVertexToWindow(entity.transformation, vertex1.position, x1, y1);

    //get second point on screen
    const Vertex& vertex2 = entity.mesh->vertices[*it++];
    int x2, y2;
    convertVertexToWindow(entity.transformation, vertex2.position, x2, y2);

    Color c(0,0,255,0);

    //get distance between two points
    Vec2 vec2(x2-x1, y2-y1);
    float magnitude = vec2.GetMagnitude();

    if (magnitude != 0.f)
    {
        vec2.toUnit();

        for (float i = 0; i <= magnitude; ++i)
        {
            Vec3 point(x1 + i * vec2.x, y1 + i * vec2.y, 0);
            pTarget->SetPixelColor(point.x, point.y, c);
        }
    }
    else 
        pTarget->SetPixelColor(x2, y2, c);
}

////////Get weight of a point in a triangle//////////
bool getWeight(const Vec2& p, const Vec2& p1, const Vec2& p2, const Vec2& p3, float* weight)
{
    float det = (p2.y - p3.y) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.y - p3.y);
    if (det == 0)
        return false;

    weight[0] = ((p2.y - p3.y) * (p.x - p3.x) + (p3.x - p2.x) * (p.y - p3.y)) / det;
    weight[1] = ((p3.y - p1.y) * (p.x - p3.x) + (p1.x - p3.x) * (p.y - p3.y)) / det;
    weight[2] = 1 - weight[1] - weight[0];
    
    return weight[0] >= 0 && weight[1] >= 0 && weight[2] >= 0;
}

Vec2 A, B, C; //TODO : TODELETE : DELETE

///////draw an horizontal line//////// O(n)
//TODO : add Vertex
void drawHorizontalLine(int x1, int x2, int height, Texture* pTarget)
{
    Color c(255, 0, 0, 255);
    float rgb[3];
    for (int x = x2; x < x1; x++)
    {
        getWeight(Vec2(x, height), A, B, C, rgb);
        c.r = rgb[0]*255;
        c.g = rgb[1]*255;
        c.b = rgb[2]*255;
        
        pTarget->SetPixelColor(x, height, c);
    }
}

//TODO : replace Vec2 by Vertex
/* 
 * if used, draw vertices as BottomFlatTriangle
 * Complexity : O(n^2)
 */
void fillBottomFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Texture* pTarget)
{
    A = v1;
    B = v2;
    C = v3;
    float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
    float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

    float curx1 = v1.x;
    float curx2 = v1.x;

    for (int scanlineY = v1.y; scanlineY <= v2.y; scanlineY++)
    {
        drawHorizontalLine((int)curx1, (int)curx2, scanlineY, pTarget);
        curx1 += invslope1;
        curx2 += invslope2;
    }
}

//TODO : replace Vec2 by Vertex
/* 
 * if used, draw vertices as BottomFlatTriangle
 * Complexity : O(n^2)
 */
void fillTopFlatTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Texture* pTarget)
{
    float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
    float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

    float curx1 = v3.x;
    float curx2 = v3.x;

    for (int scanlineY = v3.y; scanlineY > v1.y; scanlineY--)
    {
        drawHorizontalLine((int)curx1, (int)curx2, scanlineY, pTarget);
        curx1 -= invslope1;
        curx2 -= invslope2;
    }
}

/* 
 * if used, draw vertices as BottomFlatTriangle
 * Complexity : Best case  : O(n^2)
 *              Worst case : O(2 * n^2)
 */
void drawTriangle(Vec2 v1, Vec2 v2, Vec2 v3, Texture* pTarget)
{
   /* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
  //sortVerticesAscendingByY();
  //if (v1.y > v2.y && v1.y > )

  /* here we know that v1.y <= v2.y <= v3.y */
  /* check for trivial case of bottom-flat triangle */
  if (v2.y == v3.y)
  {
    fillBottomFlatTriangle(v1, v2, v3, pTarget);
  }
  /* check for trivial case of top-flat triangle */
  else if (v1.y == v2.y)
  {
    fillTopFlatTriangle(v1, v2, v3, pTarget);
  }
  else
  {
    /* general case - split the triangle in a topflat and bottom-flat one */
    Vec2 v4 = Vec2(
      (int)(v1.x + ((float)(v2.y - v1.y) / (float)(v3.y - v1.y)) * (v3.x - v1.x)), v2.y);
    fillBottomFlatTriangle(v1, v2, v4, pTarget);
    fillTopFlatTriangle(v2, v4, v3, pTarget);
  }
}

void Rasterizer::RenderTriangles(const Entity& entity, Texture* pTarget, 
                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it)
{
    const Vertex& vertex1 = entity.mesh->vertices[*it++];
    int x1, y1;
    convertVertexToWindow(entity.transformation, vertex1.position, x1, y1);

    const Vertex& vertex2 = entity.mesh->vertices[*it++];
    int x2, y2;
    convertVertexToWindow(entity.transformation, vertex2.position, x2, y2);

    const Vertex& vertex3 = entity.mesh->vertices[*it++];
    int x3, y3;
    convertVertexToWindow(entity.transformation, vertex3.position, x3, y3);


    float weight[3];
    Color c(255,0,0);

    fillBottomFlatTriangle(Vec2(x1, y1), Vec2(x2, y2), Vec2(x3, y3), pTarget);
}

void Rasterizer::RenderScene(Scene* pScene, Texture* pTarget)
{
    assert(pScene != nullptr && pTarget != nullptr);

    //init render texture
    pTarget->FillBlack();

    //render vertices of each entity
    for (const Entity& entity : pScene->entities)
    {
        std::vector<unsigned int>::iterator it = entity.mesh->indices.begin();
        while (it != entity.mesh->indices.end())
        {
            //RenderPoints(entity, pTarget, entity.mesh->vertices, it);
            //RenderLines(entity, pTarget, entity.mesh->vertices, it);
            RenderTriangles(entity, pTarget, entity.mesh->vertices, it);
        }

    }
    //TODO : 
    //Sutherrland Hardgeman algorithm
    //optimization
}
