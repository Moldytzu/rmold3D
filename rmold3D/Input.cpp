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

InputManager::InputManager()
{
    // set all mappings for common keys
    Map("Space",32)->Map("'",39)->Map(",",44)->Map("-",45)->Map(".",46)-> \
    Map("/",47)->Map("0",48)->Map("1",49)->Map("2",50)->Map("3",51)-> \
    Map("4",52)->Map("5",53)->Map("6",54)->Map("7",55)->Map("8",56)-> \
    Map("9",57)->Map(";",59)->Map("=",61)->Map("A",65)->Map("B",66)-> \
    Map("C",67)->Map("D",68)->Map("E",69)->Map("F",70)->Map("G",71)-> \
    Map("H",72)->Map("I",73)->Map("J",74)->Map("K",75)->Map("L",76)-> \
    Map("M",77)->Map("N",78)->Map("O",79)->Map("P",80)->Map("Q",81)-> \
    Map("R",82)->Map("S",83)->Map("T",84)->Map("U",85)->Map("V",86)-> \
    Map("W",87)->Map("X",88)->Map("Y",89)->Map("Z",90)->Map("[",91)-> \
    Map("\\",92)->Map("]",93)->Map("`",96)->Map("Escape",256)->Map("Enter",257)-> \
    Map("Tab",258)->Map("Backspace",259)->Map("Insert",260)->Map("Delete",261)->Map("Right",262)-> \
    Map("Left",263)->Map("Down",264)->Map("Up",265)->Map("PgUp",266)->Map("PgDown",267)-> \
    Map("Home",268)->Map("End",269)->Map("CapsLock",280)->Map("ScrollLock",281)->Map("NumLock",282)-> \
    Map("PrintScreen",283)->Map("Pause",284)->Map("F1",290)->Map("F2",291)->Map("F3",292)-> \
    Map("F4",293)->Map("F5",294)->Map("F6",295)->Map("F7",296)->Map("F8",297)-> \
    Map("F9",298)->Map("F10",299)->Map("F11",300)->Map("F12",301);
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