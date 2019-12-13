// #ifndef _RASTERIZING_VERTEX_
// #define _RASTERIZING_VERTEX_

// #include <functional>
// #include "vec3.h"
// #include "color.h"

// struct Vertex;

// struct RasterizingVertex
// {
//     Vec3 position2D = Vec3(0, 0, 0);
//     // Vec3 position3D = Vec3(0, 0, 0);
//     Vec3 normal = Vec3(0, 0, 0);
//     // Color color;

//     RasterizingVertex();
//     RasterizingVertex(const Vertex& vertex, const std::function<Vec3(const Vec3&)>& projectionFunc);

//     void setFromVertex(const Vertex& vertex, const std::function<Vec3(const Vec3&)>& projectionFunc);
// };

// #endif