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

using namespace mold::math;

float mold::math::Vfov(float Hfov, float aspectRatio) // calculate vertical fov from horizontal fov
{
    float hfovRad = Hfov * glm::pi<float>() / 180;
    float vfovRad = 2 * glm::atan(glm::tan(hfovRad / 2) * aspectRatio);
    float vfov = glm::ceil(vfovRad * 180 / glm::pi<float>());
    return glm::radians(vfov);
}