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

std::unordered_map<std::string, mold::render::objects::Component *> Components; // using it for less memory fragmentation, thus giving us a better performance

std::unordered_map<std::string, mold::render::objects::Component *> mold::render::objects::GameObjectBase::Get()
{
    std::unordered_map<std::string, mold::render::objects::Component *> tmp; // construct a map that contains only the gameobject's components
    for (auto const &[name, ptr] : Components)
    {
        if(mold::helpers::StartsWith(name,std::string(Name + " : "))) // the object's components have the name in this format: "<Name> : <Component>"
        {
            tmp.emplace(name.substr(std::string(Name + " : ").length()), ptr); // get only the component name
        }
    }
    return tmp;
}

std::unordered_map<std::string, mold::render::objects::Component *> mold::render::objects::GameObjectBase::GetRaw()
{
    return Components; // get all components
}

mold::render::objects::Component *mold::render::objects::GameObjectBase::Get(std::string name)
{
    if(!Get().count(name)) // fail if you can't find the name
    {
        mold::log::Error("Couldn't find " + name);
        return nullptr;
    }

    return Get()[name];
}

GameObjectBase *mold::render::objects::GameObjectBase::AttachComponent(std::string name, Component *component)
{
    Components.emplace(Name + " : " + name, std::move(component)); // insert the pair in the map
    component->Parent = this;                                      // set the parent to this
    component->Start();                                            // reset component
    return this;                                                   // chain
}

GameObjectBase *mold::render::objects::GameObjectBase::DettachComponent(std::string name)
{
    if (!ExistsComponent(name)) // fail if there isn't any component with that name
    {
        log::Error("Failed to dettach non-existent component");
        return this;
    }

    Components.erase(name); // erase component from the map
    return this;            // chain
}

void mold::render::objects::GameObjectBase::TickComponents()
{
    for (auto const &[name, ptr] : Components)
    {
        // don't tick disabled components
        if (ptr->Enabled && mold::helpers::StartsWith(name, (Name + std::string(" :"))))
        {
            ptr->Tick(); // tick it
        }
    }
}

void mold::render::objects::GameObjectBase::HandleComponents(mold::EventType event)
{
    for (auto const &[name, ptr] : Components)
    {
        // don't pass events to disabled components
        if (ptr->Enabled && mold::helpers::StartsWith(name, (Name + std::string(" :"))))
        {
            ptr->Handle(event); // pass event
        }
    }
}

bool mold::render::objects::GameObjectBase::ExistsComponent(std::string name)
{
    return Components.count(Name + " : " + name); // return the count of the items
}

// Constructors and distructors

mold::render::objects::GameObjectBase::GameObjectBase() : Enabled{true} {} // enable the gameobject 

mold::render::objects::GameObjectBase::~GameObjectBase() {} // do nothing

// Positioning

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Translate(glm::vec3 offset)
{
    PositionMatrix = glm::translate(PositionMatrix, offset); // translate matrix
    return this;                                             // to chain instructions
}

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Move(glm::vec3 position)
{
    PositionMatrix = glm::translate(glm::mat4(1.0f), position); // move matrix by translating a new one to the desired position
    return this;                                                // to chain instructions
}

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Scale(glm::vec3 scaleFactor)
{
    PositionMatrix = glm::scale(PositionMatrix, scaleFactor); // scale matrix
    return this;                                              // to chain instructions
}

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Rotate(glm::vec3 axis, float angle)
{
    PositionMatrix = glm::rotate(PositionMatrix, glm::radians(angle), glm::normalize(axis)); // rotate matrix
    return this;                                                                             // to chain instructions
}

glm::vec3 mold::render::objects::GameObjectBase::GetPosition()
{
    return PositionMatrix[3]; // position is stored on the 3rd axis (is this the correct term?)
}

// Wrapped

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Translate(float x, float y, float z)
{
    return Translate(glm::vec3(x, y, z)); // to chain instructions
}

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Move(float x, float y, float z)
{
    return Move(glm::vec3(x, y, z)); // to chain instructions
}

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Scale(float x, float y, float z)
{
    return Scale(glm::vec3(x, y, z)); // to chain instructions
}

mold::render::objects::GameObjectBase *mold::render::objects::GameObjectBase::Rotate(float x, float y, float z, float angle)
{
    return Rotate(glm::vec3(x, y, z), angle); // to chain instructions
}

// Run-time

void mold::render::objects::GameObjectBase::Bind()
{
    GlobalShader.Set("model", PositionMatrix);                         // give the shader our position matrix
    GlobalShader.Set("fcolour", glm::vec4(1.0f, 1.0f, 1.0f, Opacity)); // pass colour information needed for transparency
}

std::string mold::render::objects::GameObjectBase::Type()
{
    return "Empty"; // empty game object
}