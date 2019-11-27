#ifndef _MATERIAL_H_
#define _MATERIAL_H_

struct Material
{
private:

public:
    Material(){};
    ~Material(){};
    float ambient = 0.1; // [0, 1]
    float diffuse = 1.0; // [0, 1]
    float specular = 1.0; //[0, 1]
    float brillance = 100; // >> 1
};

#endif
