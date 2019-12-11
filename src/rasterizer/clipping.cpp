#include <iostream>
#include <cmath>

#include "rasterizer.h"

#include "clipping.h"

#include "vec4.h"
#include "vertex.h"
#include "rasterizingVertex.h"
#include "color.h"

#include "entity.h"

#include "macros.h"

void addClippedVertex(const Vec4& vert,
                      Color color,
                      const Vec4& normal,
                      std::vector<Vertex>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    transformedVertices.emplace_back(Vertex(vert.getHomogenizedVec(), normal, color));
    transformedIndices.emplace_back(transformedVertices.size() - 1);
}

//vertices size should be 4
void add4ClippedVertex(const Vec4* vertices,
                      const Color* color,
                      const Vec4* normal,
                      std::vector<Vertex>& transformedVertices, 
                      std::vector<unsigned int>& transformedIndices)
{
    //homogenizing
    for (unsigned int i = 0; i < 4; i++)
    {
        transformedVertices.push_back(Vertex(Rasterizer::projection(vertices[i].getHomogenizedVec()), normal[i], color[i]));
    }
    
    transformedIndices.push_back(transformedVertices.size() - 4);
    transformedIndices.push_back(transformedVertices.size() - 3);
    transformedIndices.push_back(transformedVertices.size() - 2);

    transformedIndices.push_back(transformedVertices.size() - 4);
    transformedIndices.push_back(transformedVertices.size() - 2);
    transformedIndices.push_back(transformedVertices.size() - 1);
}

// void Clipping::ClipPoints(const Entity& entity, FrameBuffer* pTarget, 
//                            const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
//                            //outputs :
//                            std::vector<Vertex>& transformedVertices, 
//                            std::vector<unsigned int>& transformedIndices)
// {
//     Vertex vertex = vertices[*it++];
    
//     vertex.position = entity.transformation * Vec4(vertex.position, 1);

//     RasterizingVertex vert;
//     vert.setFromVertex(vertex, Rasterizer::projection);

//     if (vert.position2D.x >= 0 && vert.position2D.x < windowWidth 
//     && vert.position2D.y >= 0 && vert.position2D.y < windowHeight)
//     {
//         addClippedVertex(vertex.position, vertex.color, vert.normal, transformedVertices, transformedIndices);
//     }
// }

bool Clipping::ClipPoints(const DynamicAsStaticArray<Vec3>& screenVertices, const DynamicAsStaticArray<Vertex>& worldVertices, 
                          std::vector<Vec3>& screenVerticesClipped, std::vector<Vertex>& worldVerticesClipped)
{
    //there is only one vertex for a point
    if (screenVertices[0].x >= 0 && screenVertices[0].x < windowWidth 
    && screenVertices[0].y >= 0 && screenVertices[0].y < windowHeight)
    {
        screenVerticesClipped.emplace_back(std::move(screenVertices[0]));
        worldVerticesClipped.emplace_back(std::move(worldVertices[0]));
        return false; //use current array to draw vertex
    }

    return true; //use other array (which is empty) to draw vertex (draw nothing, since there is no vertex to draw)
}

float resolve_equation(int index,float coord,float coordVect)
{
    float constante1 = 0, constante2 = 0;
    if (index == 0 || index > 3)
        constante1 = -4;
    else 
        constante1 = 4;
    if (index == 2 || index == 4)
        constante2 = 4;
    else 
        constante2 = -4;
    
    return (-(constante1*coordVect)+constante2)/constante1*coord;
}

void Clipping::ClipLines(const Entity& entity, FrameBuffer* pTarget, 
                           const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
                           //outputs :
                           std::vector<Vertex>& transformedVertices, 
                           std::vector<unsigned int>& transformedIndices)
{
//    //indices.size() % 2 must be 0
//
    Vertex vertex1 = vertices[*it++];
    Vertex vertex2 = vertices[*it++];
    Vec4 vert1 = entity.transformation * Vec4(vertex1.position, 1);
    Vec4 vert2 = entity.transformation * Vec4(vertex2.position, 1);

    Vec4 norm1 = entity.transformation * Vec4(vertex1.normal, 1);
    Vec4 norm2 = entity.transformation * Vec4(vertex2.normal, 1);

    
    if (!vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {

    }
    if (!vert1.isInsideWSizedCube() && vert2.isInsideWSizedCube())
    {
        if (vert1.x > vert1.w || vert1.x < -vert1.w)
        {
            if (vert1.x > vert1.w)
                vert1.x = vert1.w;
            else 
                vert1.x = vert1.w;
        }
        else if (vert1.y > vert1.w || vert1.y < -vert1.w)
        {
            if (vert1.x > vert1.w)
                vert1.y = vert1.w;
            else 
                vert1.y = vert1.w;
        }
        else if (vert1.z > vert1.w || vert1.z < -vert1.w)
        {
            if (vert1.x > vert1.w)
                vert1.y = vert1.w;
            else 
                vert1.y = vert1.w;
        }
    }
    else if (vert1.isInsideWSizedCube() && !vert2.isInsideWSizedCube())
    {
        float dist1 = vert1.w - vert1.x;
        float dist2 = vert2.x - vert1.w;
        float total = dist2+dist1;
        Vec4 vert3 = vert1;
        Vec4 vert4 = vert2;
        Vec4 dir = {(vert2.x-vert1.x),(vert2.y-vert1.y),(vert2.z-vert1.z),1};
        // Vec4 unit = {0,0,0,1};
        // unit.x = dir.x/ sqrt(pow(dir.x, 2) + pow(dir.y,2));
        // unit.y = dir.y/ sqrt(pow(dir.x, 2) + pow(dir.y,2));
        // unit.z = dir.z/ sqrt(pow(dir.x, 2) + pow(dir.y,2));
        //unit.Homogenize();
        if (vert2.x > 1)
        {
            vert4.x = dir.x*(dist1/total);
            vert4.y = dir.y*(dist1/total);
            vert4.z = dir.z*(dist1/total);
        }
        if (vert2.x < -1)
        {
            vert4.x = -dir.x*(dist1/total);
            vert4.y = -dir.y*(dist1/total);
            vert4.z = -dir.z*(dist1/total);
        }
        // std::cout << vert4.x <<std::endl;
        // std::cout << vert4.y <<std::endl;
        addClippedVertex(vert3,{255,255,0,0}, norm1, transformedVertices,transformedIndices);
        addClippedVertex(vert4,{255,255,0,0}, norm2, transformedVertices,transformedIndices);

    }   
}

// void Clipping::ClipTriangles(const Entity& entity, FrameBuffer* pTarget, 
//                                const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
//                                //outputs :
//                                std::vector<Vertex>& transformedVertices, 
//                                std::vector<unsigned int>& transformedIndices)
// {
//     Vertex vertex1 = vertices[*it++];
//     Vertex vertex2 = vertices[*it++];
//     Vertex vertex3 = vertices[*it++];

//     Vec4 vert1 = entity.transformation * Vec4(vertex1.position, 1);
//     Vec4 vert2 = entity.transformation * Vec4(vertex2.position, 1);
//     Vec4 vert3 = entity.transformation * Vec4(vertex3.position, 1);

//     Vec4 norm1 = entity.transformation * Vec4(vertex1.normal, 0);
//     Vec4 norm2 = entity.transformation * Vec4(vertex2.normal, 0);
//     Vec4 norm3 = entity.transformation * Vec4(vertex3.normal, 0);

//     // -w < x < w
//     // for ()

//     //if (isInsideScreen(vert1))
//     {
//         addClippedVertex(vert1, vertex1.color, norm1, transformedVertices, transformedIndices);
//         addClippedVertex(vert2, vertex2.color, norm2, transformedVertices, transformedIndices);
//         addClippedVertex(vert3, vertex3.color, norm3, transformedVertices, transformedIndices);
//     }
// }

bool Clipping::ClipTriangles(const DynamicAsStaticArray<Vec3>& screenVertices, const DynamicAsStaticArray<Vertex>& worldVertices, 
                          std::vector<Vec3>& screenVerticesClipped, std::vector<Vertex>& worldVerticesClipped)
{
    //there is only one vertex for a point
    // if (screenVertices[0].x >= 0 && screenVertices[0].x < windowWidth 
    // && screenVertices[0].y >= 0 && screenVertices[0].y < windowHeight)
    // {
        for (unsigned int i = 0; i < 3; i++)
        {
            screenVerticesClipped.emplace_back(std::move(screenVertices[i]));
            worldVerticesClipped.emplace_back(std::move(worldVertices[i]));
        }
        return false; //use current array to draw vertex
    //}

    return true; //use other array (which is empty) to draw vertex (draw nothing, since there is no vertex to draw)
}