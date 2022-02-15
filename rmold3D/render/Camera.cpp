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

using namespace mold::render::camera;

void mold::render::camera::Translate(glm::vec3 axis, float value)
{
    // translate in the desired position
    if (axis.z > 0) // forwards
        Position += value * Front;
    if (axis.z < 0) // backwards
        Position -= value * Front;
    if (axis.x < 0) // left
        Position -= glm::normalize(glm::cross(Front, Up)) * value;
    if (axis.x > 0) // right
        Position += glm::normalize(glm::cross(Front, Up)) * value;
    if (axis.y > 0) // up
        Position += glm::normalize(Up) * value;
    if (axis.y < 0) // down
        Position -= glm::normalize(Up) * value;
}

void mold::render::camera::Rotate(glm::vec3 axis, float value)
{
    if (axis.z > 0) // forwards
        Pitch += value;
    if (axis.z < 0) // backwards
        Pitch -= value;
    if (axis.x < 0) // left
        Yaw -= value;
    if (axis.x > 0) // right
        Yaw += value;
}

bool mold::render::camera::InView(glm::vec3 position)
{
    bool inY = position.y > (Position.y - settings::SkyboxDistance) && position.y < (Position.y + settings::SkyboxDistance);
    bool inX = position.x > (Position.x - settings::SkyboxDistance) && position.x < (Position.x + settings::SkyboxDistance);
    bool inZ = position.z > (Position.z - settings::SkyboxDistance) && position.z < (Position.z + settings::SkyboxDistance);
    return inY && inX && inZ;
}