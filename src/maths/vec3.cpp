#include <cmath>
#include <cassert>

#include "vec3.h"
#include "vec4.h"

std::ostream& operator<<(std::ostream& stream, const Vec3& vector)
{
    stream << "x : " << vector.x << " | y : " << vector.y << " | z : " << vector.z << std::endl;
    return stream;
}