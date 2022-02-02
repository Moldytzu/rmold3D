#include <rmold3D/mold.h>

mold::render::objects::GameObject::GameObject() {}

void mold::render::objects::GameObject::Init() {}

void mold::render::objects::GameObject::Draw() {}

void mold::render::objects::GameObject::Translate(glm::vec3 offset)
{
    PositionMatrix = glm::translate(PositionMatrix, offset);
}

void mold::render::objects::GameObject::Move(glm::vec3 position)
{
    PositionMatrix = glm::translate(glm::mat4(1.0f), position);
}

std::string mold::render::objects::GameObject::Type()
{
    return "Empty";
}

glm::vec3 mold::render::objects::GameObject::GetPosition()
{
    return PositionMatrix[3];
}