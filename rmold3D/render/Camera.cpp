#include <rmold3D/mold.h>

void mold::render::camera::Translate(mold::render::CameraDirection direction, float value)
{
    switch (direction) // translate in the desired position
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
    switch (direction) // rotate in the desired direction
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

bool mold::render::camera::InView(glm::vec3 position, float horizontalScale)
{
    float fovDiv = mold::settings::FOV/mold::settings::ViewDistanceDivisor;
    bool inY = position.y > (Position.y - fovDiv) && position.y < (Position.y + fovDiv);
    bool inX = position.x > (Position.x - fovDiv) && position.x < (Position.x + fovDiv);
    bool inZ = position.z > (Position.z - fovDiv) && position.z < (Position.z + fovDiv);
    return inY && inX && inZ;
}