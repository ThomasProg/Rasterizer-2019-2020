#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "mat4.h"
#include "vec3.h"

#include "macros.h"

class Camera
{
public:
    Camera();
    ~Camera();
    void actualize();
    Vec3 location;
    Vec3 rotation;

    static constexpr float rotationSpeed = 90 * PI / 180;
    static constexpr float translationSpeed = 15;

    const Mat4& getTransform() const 
    {
        return transform;
    };


private:
    Mat4 transform;

};

#endif