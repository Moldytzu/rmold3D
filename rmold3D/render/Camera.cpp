#include <rmold3D/mold.h>

void mold::render::camera::Translate(mold::render::CameraDirection direction, float value)
{
    switch (direction)
    {
    case mold::render::CameraDirection::Forward:
        Position += value * Front;
        break;
    case mold::render::CameraDirection::Backwards:
        Position -= value * Front;
        break;
    case mold::render::CameraDirection::Left:
        Position -= glm::normalize(glm::cross(Front, Up)) * value;
        break;
    case mold::render::CameraDirection::Right:
        Position += glm::normalize(glm::cross(Front, Up)) * value;
        break;
    default:
        break;
    }
}

void mold::render::camera::Rotate(mold::render::CameraDirection direction, float value)
{
    switch (direction)
    {
    case mold::render::CameraDirection::Forward:
        Pitch += value;
        break;
    case mold::render::CameraDirection::Backwards:
        Pitch -= value;
        break;
    case mold::render::CameraDirection::Left:
        Yaw -= value;
        break;
    case mold::render::CameraDirection::Right:
        Yaw += value;
        break;
    default:
        break;
    }
}