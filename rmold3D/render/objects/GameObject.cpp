#include <rmold3D/mold.h>

using namespace mold::render::objects;

// Components

std::unordered_map<std::string, mold::render::objects::Component *> Components;

std::unordered_map<std::string, mold::render::objects::Component *> mold::render::objects::GameObject::GetComponents()
{
    return Components;
}

void mold::render::objects::GameObject::AttachComponent(std::string name, Component *component)
{
    Components.emplace(Name + " : " + name, std::move(component));
    component->Parent = this;
    component->Start(); // reset component
}

void mold::render::objects::GameObject::DettachComponent(std::string name)
{
    if (!ExistsComponent(name))
    {
        log::Error("Failed to dettach non-existent component");
        return;
    }

    Components.erase(name); // erase component from the map
}

void mold::render::objects::GameObject::TickComponents()
{
    if (Components.size() == 0)
        return; // no components to tick
    for (auto const &[name, ptr] : Components)
    {
        // don't tick disabled components
        if (ptr->Enabled && StartsWith(name, (Name + std::string(" :"))))
        {
            ptr->Tick(); // tick it
        }
    }
}

void mold::render::objects::GameObject::HandleComponents(mold::EventType event)
{
    if (Components.size() == 0)
        return; // no components to handle
    for (auto const &[name, ptr] : Components)
    {
        // don't pass events to disabled components
        if (ptr->Enabled && StartsWith(name, (Name + std::string(" :"))))
        {
            ptr->Handle(event); // pass event
        }
    }
}

bool mold::render::objects::GameObject::ExistsComponent(std::string name)
{
    return Components.count(name);
}

// Default functions

void mold::render::objects::GameObject::Draw() {}

// Constructors and distructors

mold::render::objects::GameObject::GameObject() {}

mold::render::objects::GameObject::GameObject(mold::render::image::Texture texture)
{
    Texture = texture; // set texture and enable the object
    Enabled = true;
}

mold::render::objects::GameObject::~GameObject()
{
    Texture.Deallocate(); // deallocate texture and vabo
    Vabo.Deallocate();
}

// Positioning

mold::render::objects::GameObject *mold::render::objects::GameObject::Translate(glm::vec3 offset)
{
    PositionMatrix = glm::translate(PositionMatrix, offset); // translate matrix
    return this;                                             // to chain instructions
}

mold::render::objects::GameObject *mold::render::objects::GameObject::Move(glm::vec3 position)
{
    PositionMatrix = glm::translate(glm::mat4(1.0f), position); // move matrix by translating a new one to the desired position
    return this;                                                // to chain instructions
}

mold::render::objects::GameObject *mold::render::objects::GameObject::Scale(glm::vec3 scaleFactor)
{
    PositionMatrix = glm::scale(PositionMatrix, scaleFactor); // scale matrix
    return this;                                              // to chain instructions
}

mold::render::objects::GameObject *mold::render::objects::GameObject::Rotate(glm::vec3 axis, float angle)
{
    PositionMatrix = glm::rotate(PositionMatrix, glm::radians(angle), glm::normalize(axis)); // rotate matrix
    return this;                                                                             // to chain instructions
}

glm::vec3 mold::render::objects::GameObject::GetPosition()
{
    return PositionMatrix[3]; // position is stored on the 3rd axis (is this the correct term?)
}

// Wrapped

mold::render::objects::GameObject *mold::render::objects::GameObject::Translate(float x, float y, float z)
{
    return Translate(glm::vec3(x, y, z)); // to chain instructions
}

mold::render::objects::GameObject *mold::render::objects::GameObject::Move(float x, float y, float z)
{
    return Move(glm::vec3(x, y, z)); // to chain instructions
}

mold::render::objects::GameObject *mold::render::objects::GameObject::Scale(float x, float y, float z)
{
    return Scale(glm::vec3(x, y, z)); // to chain instructions
}

mold::render::objects::GameObject *mold::render::objects::GameObject::Rotate(float x, float y, float z, float angle)
{
    return Rotate(glm::vec3(x, y, z), angle); // to chain instructions
}

// Texturing

void mold::render::objects::GameObject::ReplaceTexture(mold::render::image::Texture newTexture)
{
    Texture.Deallocate(); // deallocate so we don't create memory leaks or smth
    Texture = newTexture; // replace the old texture
}

// Run-time

void mold::render::objects::GameObject::Bind()
{
    Vabo.Bind();                                                       // bind vao & vbo
    Texture.Bind();                                                    // bind texture
    GlobalShader.Set("model", PositionMatrix);                         // give the shader our position matrix
    GlobalShader.Set("fcolour", glm::vec4(1.0f, 1.0f, 1.0f, Opacity)); // pass colour information needed for transparency
    GlobalShader.Set("objPos", PositionMatrix[3]);
}

std::string mold::render::objects::GameObject::Type()
{
    return "Empty"; // empty game object
}