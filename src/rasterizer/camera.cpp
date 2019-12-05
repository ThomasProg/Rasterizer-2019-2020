#include "camera.h"
#include "vec4.h"

Camera::Camera()
    // : location(0,0,30),
    //   rotation(-0.3f,0,0),
    : location(0,0,30),
      rotation(0,0,0),
      transform(Mat4::CreateTranslationMatrix(location))
{

}

Camera::~Camera(){}

void Camera::actualize()
{
    transform = Mat4::CreateRotationMatrix(rotation)*Mat4::CreateTranslationMatrix(location);
}
