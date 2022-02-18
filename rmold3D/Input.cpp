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