#include "Camera.h"

// Setter definitions
void Camera::setAspectRatio(smath::vec2 dimensions)
{
    // Calulcating aspect ratio
    float aspectRatio = (dimensions.x = 0.0f ? 0.0f : dimensions.y / dimensions.x); // Checking if dividing by 0

    // Setting aspect ratio
    mAspectRatio = aspectRatio;
}

// Gette definitions
smath::mat4 Camera::getProjectionMatrix()
{
    smath::mat4 projectionMatrix = smath::mat4();
    projectionMatrix *= smath::scale(smath::vec3(1.0f) / smath::vec3(mScale, mScale * mAspectRatio, 1.0f));
    projectionMatrix *= smath::rotateZ(smath::DEG2RAD * mRotation);
    projectionMatrix *= smath::translate(smath::vec3(mPosition, 0.0f));

    return projectionMatrix;
}