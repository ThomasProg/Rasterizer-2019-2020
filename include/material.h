#ifndef _MATERIAL_H_
#define _MATERIAL_H_

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
};

#endif
