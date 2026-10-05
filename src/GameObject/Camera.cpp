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
    projectionMatrix *= smath::orthographic(
        mPosition.x - mScale,
        mPosition.x + mScale,
        mPosition.y - mAspectRatio * mScale,
        mPosition.y + mAspectRatio * mScale,
        0.0f, 1.0f
    );
    return projectionMatrix;
}