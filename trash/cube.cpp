#include "cube.h"
#include "mesh.h"

void Construct::cube(Mesh& mesh)
{
    for (float i = -0.5; i < 1; i++)
    {
        mesh.addQuad(
            Vector3(i, -0.5, -0.5), 
            Vector3(i, -0.5, 0.5), 
            Vector3(i, 0.5, 0.5),
            Vector3(i, 0.5, -0.5));

        mesh.addQuad(
            Vector3(-0.5, i, -0.5), 
            Vector3(-0.5, i, 0.5), 
            Vector3(0.5, i, 0.5),
            Vector3(0.5, i, -0.5));

        mesh.addQuad(
            Vector3(-0.5, -0.5, i), 
            Vector3(-0.5, 0.5, i), 
            Vector3(0.5, 0.5, i),
            Vector3(0.5, -0.5, i));
    }
}