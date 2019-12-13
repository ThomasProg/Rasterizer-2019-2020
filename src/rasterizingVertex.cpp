// #include "rasterizingVertex.h"

// #include "vertex.h"

// RasterizingVertex::RasterizingVertex()
    
// {

// }

// RasterizingVertex::RasterizingVertex(const Vertex& vertex, const std::function<Vec3(const Vec3&)>& projectionFunc)
//     : position2D(vertex.position),
//       position3D(projectionFunc(vertex.position)),
//       normal(vertex.normal),
//       color(vertex.color)
// {

// }

// void RasterizingVertex::setFromVertex(const Vertex& vertex, const std::function<Vec3(const Vec3&)>& projectionFunc)
// {
//     position3D  = vertex.position;
//     position2D  = projectionFunc(vertex.position);
//     normal      = vertex.normal;
//     color       = vertex.color;
// }