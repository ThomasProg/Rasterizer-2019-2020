#ifndef _MATERIAL_H_
#define _MATERIAL_H_

struct Material
{
private:

public:
    Material(){};
    ~Material(){};
    float ambient = 0.5; // [0, 1]
    float diffuse = 1.0; // [0, 1]
    float specular = 0.4; //[0, 1]
    float brillance = 10; // >> 1
};

#endif