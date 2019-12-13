#include <cmath>
#include <cassert>

#include "vec4.h"
#include "vec3.h"

std::ostream& operator<<(std::ostream& stream, const Vec4& vector)
{
    stream << "x : " << vector.x << " | y : " << vector.y << " | z : " << vector.z << " | w : " << vector.w << std::endl;
    return stream;
}

