#ifndef _CAMERA_H_
#define _CAMERA_H_
#include "mat4.h"
#include "vec3.h"

#include "macros.h"

class GLFWwindow;

class Camera
{
private:
    Mat4 transform;

public:
    float prevMouseLocX = 0, prevMouseLocY = 0;

    //cartesian coordinates
    Vec3 cartesianLocation = Vec3(0, 0, 30);
    Vec3 cartesianRotation = Vec3(0, 0, 0);

    //spherical coordinates
    float spherialRadius = 30.f;
    Vec3  sphericalRotation = Vec3(0, 0, 0);

    static constexpr float rotationSpeed = 0.08;
    static constexpr float translationSpeed = 10;

public:

    Camera();
    ~Camera();
    void actualize();

    void inputs(float deltaTime, GLFWwindow* window);
    __inline const Mat4& getTransform() noexcept const  { return transform; };
};

#endif