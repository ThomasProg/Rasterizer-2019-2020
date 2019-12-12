#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <functional>

class Color;
class Vec3;

struct Material
{
private:

public:
    Material(){};
    ~Material(){};
    float ambient = 0.3; // [0, 1]
    float diffuse = 0.4; // [0, 1]
    float specular = 0.7; //[0, 1]
    float brillance = 100; // >> 1

    std::function<void(Color&, Vec3&)> additionalShaders = [](Color&, Vec3&){};
};

#endif
