#include <rmold3D/mold.h>

mold::render::objects::GameObject::GameObject() {}
void mold::render::objects::GameObject::Init() {}
void mold::render::objects::GameObject::Draw() {}

void mold::render::objects::GameObject::Translate(glm::vec3 offset)
{
    Position = glm::translate(Position,offset);
}

void mold::render::objects::GameObject::Move(glm::vec3 position)
{
    Position = glm::translate(glm::mat4(1.0f),position);
}