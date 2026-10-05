#ifndef CAMERA
#pragma once

#include <smath/smath.h>

struct Camera
{
    // Constructor
    Camera(smath::vec2 position = smath::vec2(0), float scale = 10.0f, float rotation = 0.0f, float aspectRatio = 1.0f)
        : mPosition(position), mAspectRatio(aspectRatio), mScale(scale), mRotation(rotation) {}

    // Setters
    void setAspectRatio(smath::vec2 dimensions);
    void setAspectRatio(float width, float height) { setAspectRatio(smath::vec2(width, height)); }

    // Getters
    smath::mat4 getProjectionMatrix();

    // Member variables
    smath::vec2 mPosition;
    float mScale;
    float mRotation;
    float mAspectRatio;
};

#endif // !CAMERA
