#include "sphere.h"
#include "mesh.h"

//meridians = horizontalRotation subUnits
//parallels = verticalRotation subUnits
void Construct::UVSphere(unsigned int meridians, unsigned int parallels, Mesh &mesh)
{
    mesh.vertices.reserve(2 + meridians * (parallels - 1)); 
    mesh.triangles.reserve(2 * (meridians + 1) * (parallels - 1));

    for (unsigned int i = 0; i < meridians; ++i)
    {
        {
            //pole 1
            unsigned int const a = i + 1;
            unsigned int const b = (i + 1) % meridians + 1;
            mesh.addTriangle(0, b, a);
        }
    }

    mesh.vertices.emplace_back(0.0f, 1.f, 0.0f);

    for (unsigned int j = 0; j < parallels - 1; ++j)
    {
        //optimization : outside of the last loop
        double const polar = M_PI * double(j+1) / double(parallels);
        double const sinVertical = std::sin(polar);
        double const cosVertical = std::cos(polar);

        unsigned int aStart = j * meridians + 1;
        unsigned int bStart = (j + 1) * meridians + 1;
        for (unsigned int i = 0; i < meridians; ++i)
        {
            double const azimuth = 2.0 * M_PI * double(i) / double(meridians);
            double const sinHorizontal = std::sin(azimuth);
            double const cosHorizontal = std::cos(azimuth);

            //any point is the combination of 2 angles
            double const x = sinVertical * cosHorizontal;
            double const y = cosVertical;
            double const z = sinVertical * sinHorizontal;
            //add vertice location
            mesh.vertices.emplace_back(x, y, z);

            if (j < parallels - 2)
            {
                //sphere body triangle indices
                const unsigned int a = aStart + i;
                const unsigned int a1 = aStart + (i + 1) % meridians;
                const unsigned int b = bStart + i;
                const unsigned int b1 = bStart + (i + 1) % meridians;
                mesh.addQuad(a, a1, b1, b);
            }
        }
    }

    mesh.vertices.emplace_back(0.0f, -1.f, 0.0f);

    //sphere poles triangle indices
    for (unsigned int i = 0; i < meridians; ++i)
    {
        {
            //pole 2
            unsigned int const a = i + meridians * (parallels - 2) + 1;
            unsigned int const b = (i + 1) % meridians + meridians * (parallels - 2) + 1;
            mesh.addTriangle(mesh.vertices.size() - 1, a, b);
        }
    }
}