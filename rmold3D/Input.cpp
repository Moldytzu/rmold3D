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

mold::CursorLockingMode oldLockingMode;
void mold::input::HandleCursor()
{
    if (oldLockingMode != input::GlobalCursorLockMode)
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
            glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
        oldLockingMode = input::GlobalCursorLockMode; // save old value
    }
}

double lastX,lastY;
void mold::input::HandleMouse()
{
    // handle mouse input
    double xpos, ypos;
    glfwGetCursorPos(GlobalWindow, &xpos, &ypos); // get cursor position

    // wrap cursor around window borders
    if (input::GlobalCursorLockMode == CursorLockingMode::Wrapped)
    {
        if (xpos > settings::WindowWidth)
        {
            glfwSetCursorPos(GlobalWindow, 10, ypos);
            lastX = 10;
            return;
        }

        if (xpos < 0)
        {
            glfwSetCursorPos(GlobalWindow, settings::WindowWidth - 10, ypos);
            lastX = settings::WindowWidth - 10;
            return;
        }

        if (ypos > settings::WindowHeight)
        {
            glfwSetCursorPos(GlobalWindow, xpos, 10);
            lastY = 10;
            return;
        }

        if (ypos < 0)
        {
            glfwSetCursorPos(GlobalWindow, xpos, settings::WindowHeight - 10);
            lastY = settings::WindowHeight - 10;
            return;
        }
    }

    // set axis data
    input::GlobalCursorAxis.x = xpos - lastX;
    input::GlobalCursorAxis.y = lastY - ypos;

    input::GlobalCursorPos.x = xpos;
    input::GlobalCursorPos.y = ypos;

    // call event if we've got changes
    if ((input::GlobalCursorAxis.x != 0 && input::GlobalCursorAxis.y != 0) || input::GlobalScrollAxis != 0)
        GlobalEventSystem.CallEvent(EventType::Mouse);

    // update last values
    lastX = xpos;
    lastY = ypos;
    input::GlobalScrollAxis = 0; // reset scroll value
}