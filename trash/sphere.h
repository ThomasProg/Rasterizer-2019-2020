#ifndef _SPHERE_H_
#define _SPHERE_H_

#include "macros.h"

struct Mesh;

namespace Construct
{
    //meridians = horizontalRotation subUnits
    //parallels = verticalRotation subUnits
    void UVSphere(unsigned int meridians, unsigned int parallels, Mesh &mesh);
};

#endif