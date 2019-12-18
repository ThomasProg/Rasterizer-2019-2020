#include <cmath>
#include <GLFW/glfw3.h>
#include "camera.h"
#include "vec4.h"

Camera::Camera()
    // : location(0,0,30),
    //   rotation(0,0,0),
      //transform(Mat4::CreateTranslationMatrix(cartesianLocation))
{

}

Camera::~Camera(){}

void Camera::actualize()
{
    //opti with sin and cos

    // cartesianLocation.x = spherialRadius * std::sin(sphericalRotation.x) * std::cos(sphericalRotation.y);
    // cartesianLocation.y = spherialRadius * std::sin(sphericalRotation.x) * std::sin(sphericalRotation.y);
    // cartesianLocation.z = spherialRadius * std::cos(sphericalRotation.x);
    //std::cout << cartesianLocation << std::endl;

    transform = Mat4::CreateTranslationMatrix(cartesianLocation) * Mat4::CreateRotationMatrix(cartesianRotation);
}

template<class T>
constexpr const T& clamp( const T& v, const T& lo, const T& hi )
{
    assert( !(hi < lo) );
    return (v < lo) ? lo : (hi < v) ? hi : v;
}

void Camera::inputs(float deltaTime, GLFWwindow* window)
{
    #ifdef __FIRST_PERSON__

    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    //faster on diagonales : to fix
    if (glfwGetKey(window, GLFW_KEY_UP) || glfwGetKey(window, GLFW_KEY_W))
    {
        cartesianLocation.x += translationSpeed * sin(-cartesianRotation.y) * deltaTime;
        cartesianLocation.z -= translationSpeed * cos(-cartesianRotation.y) * deltaTime;
        //cartesianLocation.z -= translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) || glfwGetKey(window, GLFW_KEY_S))
    {
        cartesianLocation.x -= translationSpeed * sin(-cartesianRotation.y) * deltaTime;
        cartesianLocation.z += translationSpeed * cos(-cartesianRotation.y) * deltaTime;
        //cartesianLocation.z += translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_RIGHT) || glfwGetKey(window, GLFW_KEY_D))
    {
        cartesianLocation.x += translationSpeed * sin(-cartesianRotation.y + PI/2) * deltaTime;
        cartesianLocation.z -= translationSpeed * cos(-cartesianRotation.y + PI/2) * deltaTime;
        //cartesianLocation.x += translationSpeed * deltaTime;
    }
    if (glfwGetKey(window, GLFW_KEY_LEFT) || glfwGetKey(window, GLFW_KEY_A))
    {
        cartesianLocation.x -= translationSpeed * sin(-cartesianRotation.y + PI/2) * deltaTime;
        cartesianLocation.z += translationSpeed * cos(-cartesianRotation.y + PI/2) * deltaTime;
        //cartesianLocation.x -= translationSpeed * deltaTime;
    }

    if (!bFirstInput)
    {
        float deltaMouseX = prevMouseLocX - mouseX;
        float deltaMouseY = prevMouseLocY - mouseY; 

        if (glfwGetKey(window, GLFW_KEY_I))
            deltaMouseY = rotationSpeedOnKey;
        if (glfwGetKey(window, GLFW_KEY_J))
            deltaMouseX = rotationSpeedOnKey;
        if (glfwGetKey(window, GLFW_KEY_K))
            deltaMouseY = -rotationSpeedOnKey;
        if (glfwGetKey(window, GLFW_KEY_L))
            deltaMouseX = -rotationSpeedOnKey;

        cartesianRotation.y += deltaMouseX * rotationSpeed * deltaTime;
        cartesianRotation.x = clamp(cartesianRotation.x + deltaMouseY * rotationSpeed * deltaTime, - PI / 2, PI / 2);
    }
    else 
        bFirstInput = false;
        
    prevMouseLocX = mouseX;
    prevMouseLocY = mouseY;

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT))
        cartesianLocation.y -= translationSpeed * deltaTime;
    if (glfwGetKey(window, GLFW_KEY_SPACE))
        cartesianLocation.y += translationSpeed * deltaTime;
         
    #endif
}