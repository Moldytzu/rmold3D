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

Light::Light() {}
Light::Light(glm::vec3 pos) : Position{pos}, Colour{glm::vec3(1)}, Power{1}, Shinniness{1} {}
Light::Light(glm::vec3 pos, glm::vec3 col) : Position{pos}, Colour{col}, Power{1}, Shinniness{1} {}
Light::Light(glm::vec3 pos, glm::vec3 col, float power, float shinniness) : Position{pos}, Colour{col}, Power{power}, Shinniness{shinniness} {}

void Light::Bind()
{
    GlobalShader.Set("lightColour",Colour);
    GlobalShader.Set("lightPosition",Position+glm::vec3(1));
    GlobalShader.Set("lightPower",Power);
    GlobalShader.Set("lightShininess",Shinniness);
}