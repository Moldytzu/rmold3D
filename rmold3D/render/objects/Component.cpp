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

// virtual functions that have to be overriden by the game
void mold::render::objects::Component::Handle(EventType event) {}
void mold::render::objects::Component::Tick() {}
void mold::render::objects::Component::Start() {}

void mold::render::objects::Component::CallFunc(std::string publicName)
{
    if(!Public.count(publicName)) // if there isn't any function with that name just fail
    {
        mold::log::Error("Couldn't find function " + publicName);
        return;
    }
    std::any_cast<void(*)(mold::render::objects::Component *)>(Public[publicName])(this); // call the function with the public name
}

void mold::render::objects::Component::Expose(void (*fn)(mold::render::objects::Component *), std::string publicName)
{
    Public[publicName] = fn;
}