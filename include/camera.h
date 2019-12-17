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
    bool bFirstInput = true;

public:
    float prevMouseLocX = 0, prevMouseLocY = 0;

    //cartesian coordinates
    Vec3 cartesianLocation = Vec3(0, 0, 16);
    Vec3 cartesianRotation = Vec3(0, 0, 0);

    // //spherical coordinates
    // float spherialRadius = 30.f;
    // Vec3  sphericalRotation = Vec3(0, 0, 0);

    static constexpr float rotationSpeed = 0.05;
    static constexpr float translationSpeed = 10;
    static constexpr float rotationSpeedOnKey = 15.f;

public:

    Camera();
    ~Camera();
    void actualize();

    void inputs(float deltaTime, GLFWwindow* window);
    __inline const Mat4& getTransform() const noexcept  { return transform; };
};

#endif