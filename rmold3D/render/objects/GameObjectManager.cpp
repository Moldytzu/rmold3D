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

std::unordered_map<std::string, uint32_t> newObjects;

void mold::render::objects::GameObjectsManager::Add(std::string name, GameObject *object)
{
    object->Name = name;
    GameObjects.emplace(std::move(name), std::move(object)); // build std::pair and insert it in the map
}

mold::render::objects::GameObject *mold::render::objects::GameObjectsManager::Instantiate(GameObject *object)
{
    return Instantiate(object, object->Type()); // instantiate with the object's type as name
}

mold::render::objects::GameObject *mold::render::objects::GameObjectsManager::Instantiate(GameObject *object, std::string name)
{
    std::string tname = name; // create a string based on the object's type

    if (newObjects[name] > 0)
        tname += " " + std::to_string(newObjects[name]); // append the object's no' if there are more then 1 object of that type

    Add(tname, object); // add our new object to our map/database with the other game objects
    newObjects[name]++; // increment the no' of instantiated objects with that type

    return object; // chain
}

bool mold::render::objects::GameObjectsManager::Exists(std::string name)
{
    // check if map contains the game object's name
    return Get().count(name);
}

void mold::render::objects::GameObjectsManager::Remove(std::string name)
{
    if (!Exists(name))
    {
        log::Warn("Can't remove inexistent game object " + name);
        return;
    }

    GameObjects.erase(name); // erase name if it exists
}

mold::render::objects::GameObject *mold::render::objects::GameObjectsManager::Get(std::string name)
{
    if (!Exists(name))
    {
        log::Warn("Can't get inexistent game object " + name);
        return nullptr;
    }

    return GameObjects[name]; // get if exists
}

std::unordered_map<std::string, mold::render::objects::GameObject *> mold::render::objects::GameObjectsManager::Get()
{
    return GameObjects; // return map
}