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

#include "../mold.h"

using namespace mold::render;

// glfw window callbacks

void mold::render::OnResize(GLFWwindow *window, int width, int height) // it is called when the window is being resized
{
    Events::CallEvent(EventType::Resize); // call resize event
    settings::WindowHeight = height;      // set new window height and width
    settings::WindowWidth = height;
    glViewport(0, 0, width, height);                                   // set new viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // reset orthography
}

void mold::render::OnScroll(GLFWwindow *window, double xoffset, double yoffset) // it is called when the scroll wheel is used
{
    input::GlobalScrollAxis = yoffset; // the scroll wheel in glfw works as a mouse where the yoffset is the cursor axis itself
}

double lastX, lastY;
void mold::render::OnMouse(GLFWwindow *window, double xoffset, double yoffset) // it is called when the mouse is moved
{
    // set axis data
    input::GlobalCursorAxis.x = xoffset - lastX;
    input::GlobalCursorAxis.y = lastY - yoffset;

    input::GlobalCursorPos.x = xoffset;
    input::GlobalCursorPos.y = yoffset;

    // call event if we've got changes
    if ((input::GlobalCursorAxis.x != 0 && input::GlobalCursorAxis.y != 0) || input::GlobalScrollAxis != 0)
        Events::CallEvent(EventType::Mouse);

    // update last values
    lastX = xoffset;
    lastY = yoffset;
    input::GlobalScrollAxis = 0; // reset scroll value
}