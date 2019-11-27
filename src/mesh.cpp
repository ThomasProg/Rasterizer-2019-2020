#include "mesh.h"

#include <cmath>
#include "macros.h"

void Mesh::addTriangle(unsigned int indice1, unsigned int indice2, unsigned int indice3)
{
    indices.push_back(indice1);
    indices.push_back(indice2);
    indices.push_back(indice3);
}

void Mesh::addQuad(int indice1, int indice2, int indice3, int indice4)
{
    // 2______1
    // |     /|
    // |   /  |
    // | /    |
    // 3______4

    indices.push_back(indice1);
    indices.push_back(indice2);
    indices.push_back(indice3);

    indices.push_back(indice1);
    indices.push_back(indice3);
    indices.push_back(indice4);
}

Mesh* Mesh::CreateCube()
{
    //    6______ 5
    //   /      /|          y+
    // 0_____1/  |          |
    // |     |   |          |
    // |     |   |4         |
    // |     |  /           |
    // 2_____3/             z+------> x+

    Mesh* mesh = new Mesh;
    mesh->vertices.reserve(8);
    //mesh->normals.reserve(8);
    mesh->indices.reserve(36);

    constexpr float min = -0.5;
    constexpr float max = 0.5;

#define ADDING_VERTEX_AND_NORMAL(vec) \
    mesh->vertices.emplace_back(Vertex(vec, vec));
    //mesh->normals.emplace_back(vec);

    ADDING_VERTEX_AND_NORMAL(Vec3(min, max, max));
    ADDING_VERTEX_AND_NORMAL(Vec3(max, max, max));
    ADDING_VERTEX_AND_NORMAL(Vec3(min, min, max));
    ADDING_VERTEX_AND_NORMAL(Vec3(max, min, max));

    ADDING_VERTEX_AND_NORMAL(Vec3(max, min, min));
    ADDING_VERTEX_AND_NORMAL(Vec3(max, max, min));
    ADDING_VERTEX_AND_NORMAL(Vec3(min, max, min));
    ADDING_VERTEX_AND_NORMAL(Vec3(min, min, min));

#undef ADDING_VERTEX_AND_NORMAL

    //visible faces (see schema)
    mesh->addQuad(1, 5, 4, 3); //x+
    mesh->addQuad(0, 6, 5, 1); //y+
    mesh->addQuad(0, 1, 3, 2); //z+

    //hidden faces (see schema)
    mesh->addQuad(0, 2, 7, 6); //x-
    mesh->addQuad(2, 3, 4, 7); //y-
    mesh->addQuad(6, 7, 4, 5); //z- 

    return mesh;
}

Mesh* Mesh::CreateSphere(unsigned int latitudeCount, unsigned int longitudeCount)
{
    Mesh* mesh = new Mesh;
    mesh->vertices.reserve(2 + latitudeCount * (longitudeCount - 1)); 
    mesh->indices.reserve(2 * (latitudeCount + 1) * (longitudeCount - 1));

    for (unsigned int i = 0; i < latitudeCount; ++i)
    {
        {
            //pole 1
            unsigned int const a = i + 1;
            unsigned int const b = (i + 1) % latitudeCount + 1;
            mesh->addTriangle(0, b, a);
        }
    }

    mesh->vertices.emplace_back(Vec3(0.0f, 1.f/2, 0.0f), Vec3(0.0f, 1.f/2, 0.0f));

    for (unsigned int j = 0; j < longitudeCount - 1; ++j)
    {
        //optimization : outside of the last loop
        double const polar = PI * double(j+1) / double(longitudeCount);
        double const sinVertical = std::sin(polar);
        double const cosVertical = std::cos(polar);

        unsigned int aStart = j * latitudeCount + 1;
        unsigned int bStart = (j + 1) * latitudeCount + 1;
        for (unsigned int i = 0; i < latitudeCount; ++i)
        {
            double const azimuth = 2.0 * M_PI * double(i) / double(latitudeCount);
            double const sinHorizontal = std::sin(azimuth);
            double const cosHorizontal = std::cos(azimuth);

            //any point is the combination of 2 angles
            double const x = sinVertical * cosHorizontal;
            double const y = cosVertical;
            double const z = sinVertical * sinHorizontal;
            //add vertice location
            //divide by 2 to put the sphere coordinates between -0.5 and 1
            mesh->vertices.emplace_back(Vertex(Vec3(x/2, y/2, z/2), Vec3(x/2, y/2, z/2))); 
            //mesh->normals.emplace_back(Vec3(x/2, y/2, z/2));

            if (j < longitudeCount - 2)
            {
                //sphere body triangle indices
                const unsigned int a = aStart + i;
                const unsigned int a1 = aStart + (i + 1) % latitudeCount;
                const unsigned int b = bStart + i;
                const unsigned int b1 = bStart + (i + 1) % latitudeCount;
                mesh->addQuad(a, a1, b1, b);
            }
        }
    }

    mesh->vertices.emplace_back(Vec3(0.0f, -1.f/2, 0.0f), Vec3(0.0f, -1.f/2, 0.0f));

    //sphere poles triangle indices
    for (unsigned int i = 0; i < latitudeCount; ++i)
    {
        {
            //pole 2
            unsigned int const a = i + latitudeCount * (longitudeCount - 2) + 1;
            unsigned int const b = (i + 1) % latitudeCount + latitudeCount * (longitudeCount - 2) + 1;
            mesh->addTriangle(mesh->vertices.size() - 1, a, b);
        }
    }

    return mesh;
}

Mesh* Mesh::CreateTriangle(Vec3 point1, Vec3 point2, Vec3 point3)
{
    Mesh* mesh = new Mesh;
    mesh->vertices.reserve(3);
    //mesh->normals.reserve(8);
    mesh->indices.reserve(3);

    constexpr float min = -0.5;
    constexpr float max = 0.5;

#define ADDING_VERTEX_AND_NORMAL(vec) \
    mesh->vertices.emplace_back(Vertex(vec, vec));

    // ADDING_VERTEX_AND_NORMAL(Vec3(min, max, max));
    // ADDING_VERTEX_AND_NORMAL(Vec3(max, max, max));
    // ADDING_VERTEX_AND_NORMAL(Vec3(max, min, max));

    ADDING_VERTEX_AND_NORMAL(point1);
    ADDING_VERTEX_AND_NORMAL(point2);
    ADDING_VERTEX_AND_NORMAL(point3);

    // ADDING_VERTEX_AND_NORMAL(Vec3(max, min, min));
    // ADDING_VERTEX_AND_NORMAL(Vec3(max, max, min));
    // ADDING_VERTEX_AND_NORMAL(Vec3(min, max, min));
    // ADDING_VERTEX_AND_NORMAL(Vec3(min, min, min));

#undef ADDING_VERTEX_AND_NORMAL

    //visible faces (see schema)
    mesh->addTriangle(0, 1, 2); //z+

    //hidden faces (see schema)
    //mesh->addQuad(6, 7, 4, 5); //z- 

    return mesh;
}