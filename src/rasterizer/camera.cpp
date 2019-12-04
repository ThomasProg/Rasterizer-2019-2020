#include "camera.h"


Camera::Camera()
    : location(0,0,10),
      rotation(0,0,0),
      transform(Mat4::CreateTranslationMatrix(location))
{}

Camera::~Camera(){}

void Camera::actualize()
{
    transform = Mat4::CreateRotationMatrix(rotation)*Mat4::CreateTranslationMatrix(location);
}
