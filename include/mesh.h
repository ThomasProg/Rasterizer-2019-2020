#ifndef _MESH_H_
#define _MESH_H_

#include <vector>
#include "vertex.h"
#include "vec3.h"

struct Mesh
{
private:
    void addTriangle(unsigned int indice1, unsigned int indice2, unsigned int indice3);

    //indice1 and indice indice3 should be draw border between the two triangles
    void addQuad(int indice1, int indice2, int indice3, int indice4);
    
public:
    std::vector<Vertex> vertices; //vertex buffer
    //should not change when the objects is transformed

    std::vector<unsigned int> indices; //index buffer
    //goes by triplets to draw triangles

    // std::vector<Vec3> normals; //index buffer
    // //goes by triplets to draw triangles

public:
    static Mesh* CreateCube(); //cube of size 1*1*1
    static Mesh* CreateSphere(unsigned int latitudeCount, unsigned int longitudeCount); //sphere of radius 1
    static Mesh* CreateTriangle(Vec3 point1, Vec3 point2, Vec3 point3);
};

#endif