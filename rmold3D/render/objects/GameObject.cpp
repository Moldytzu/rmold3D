/*
    rmmold3D
    Copyright (C) 2022  Moldovan Alexandru

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../../mold.h"

using namespace mold::render::objects;

// Components

std::unordered_map<std::string, mold::render::objects::Component *> Components;

std::unordered_map<std::string, mold::render::objects::Component *> mold::render::objects::GameObject::GetComponents()
{
    return Components;
}

GameObject *mold::render::objects::GameObject::AttachComponent(std::string name, Component *component)
{
    Components.emplace(Name + " : " + name, std::move(component)); // insert the pair in the map
    component->Parent = this;                                      // set the parent to this
    component->Start();                                            // reset component
    return this;                                                   // chain
}

GameObject *mold::render::objects::GameObject::DettachComponent(std::string name)
{
    if (!ExistsComponent(name)) // fail if there isn't any component with that name
    {
        log::Error("Failed to dettach non-existent component");
        return this;
    }

    Components.erase(name); // erase component from the map
    return this;            // chain
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
    return Components.count(name); // return the count of the items
}

// Default functions

void mold::render::objects::GameObject::Draw() {}

// Constructors and distructors

mold::render::objects::GameObject::GameObject() {}

mold::render::objects::GameObject::GameObject(mold::render::image::Texture texture) : Texture{texture}, Enabled{true} {} // set texture and enable the object

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
}

std::string mold::render::objects::GameObject::Type()
{
    return "Empty"; // empty game object
}