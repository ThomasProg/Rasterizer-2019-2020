// #ifndef _CLIPPING_H_
// #define _CLIPPING_H_

// #include <vector>

// #include "dynamicAsStaticArray.h"

// class Entity;

// struct FrameBuffer;
// struct Vertex;

// class Clipping
// {
// public:
//     // static void ClipTriangles(const Entity& entity, FrameBuffer* pTarget, 
//     //                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it,
//     //                             //outputs :
//     //                             std::vector<Vertex>& transformedVertices, 
//     //                             std::vector<unsigned int>& transformedIndices);
//     static bool ClipTriangles(const DynamicAsStaticArray<Vec3>& screenVertices, const DynamicAsStaticArray<Vertex>& worldVertices, 
//                           std::vector<Vec3>& screenVerticesClipped, std::vector<Vertex>& worldVerticesClipped);

//     static void ClipLines(const Entity& entity, FrameBuffer* pTarget, 
//                             const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
//                             //outputs :
//                             std::vector<Vertex>& transformedVertices, 
//                             std::vector<unsigned int>& transformedIndices);

//     // static void ClipPoints(const Entity& entity, FrameBuffer* pTarget, 
//     //                         const std::vector<Vertex>& vertices, std::vector<unsigned int>::iterator& it, //iterator of indices
//     //                         //outputs :
//     //                         std::vector<Vertex>& transformedVertices, 
//     //                         std::vector<unsigned int>& transformedIndices);

//     static bool ClipPoints(const DynamicAsStaticArray<Vec3>& screenVertices, const DynamicAsStaticArray<Vertex>& worldVertices, 
//                            std::vector<Vec3>& screenVerticesClipped, std::vector<Vertex>& worldVerticesClipped);
// };

// #endif