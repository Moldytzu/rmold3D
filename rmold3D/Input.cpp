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

#include "mold.h"

using namespace mold::input;

InputManager *InputManager::Map(std::string mapping, int key)
{
    if(!Mappings.count(mapping)) // if there isn't any item then insert it
        Mappings.emplace(mapping,key);
    else // if exists then just overwrite the old value
        Mappings[mapping] = key;

    return this; // for chaining
}

InputManager *InputManager::UnMap(std::string mapping)
{
    if(!Mappings.count(mapping)) // if there isn't any mapping with that name just error out
    {
        mold::log::Error("Failed to unmap " + mapping + " since it doesn't exist");
        return this;
    }

    Mappings.erase(mapping); // erase mapping

    return this; // for chaining
}

float InputManager::Get(std::string mapping) // get axis (0.0f - 1.0f) 
{
    if(!Mappings.count(mapping)) // if there isn't any mapping with that name just error out
    {
        mold::log::Error("Failed to get " + mapping + " since it doesn't exist");
        return 0.0f;
    }

    int key = Mappings[mapping]; // get the mapping

    if (key < 32) // 0-31 are reserved for future use (like joystick, mouse etc.)
    {
        return 0.0f; // just return 0 for now
    }
    else // the rest are glfw keys so we just call glfwGetKey
        return (float)glfwGetKey(GlobalWindow, key);
}

bool mold::input::GetKey(int key)
{
    return glfwGetKey(GlobalWindow, key); // wrap glfwGetKey
}

void mold::input::HandleCursor()
{
    switch (input::GlobalCursorLockMode)
    {
    case CursorLockingMode::Normal:
        glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    case CursorLockingMode::Locked:
        glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    case CursorLockingMode::Centred:
        glfwSetCursorPos(GlobalWindow, settings::WindowWidth / 2, settings::WindowHeight / 2);
        glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        break;
    default:
        glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        break;
    }
}