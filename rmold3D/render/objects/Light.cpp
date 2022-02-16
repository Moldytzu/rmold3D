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

Light::Light() : GameObject(image::Texture(render::Colour(0))) {}
Light::Light(glm::vec3 pos) : GameObject(image::Texture(render::Colour(0))), Position{pos}, Colour{glm::vec3(1)}, Power{1} {}
Light::Light(glm::vec3 pos, glm::vec3 col) : GameObject(image::Texture(render::Colour(0))), Position{pos}, Colour{col}, Power{1} {}
Light::Light(glm::vec3 pos, glm::vec3 col, float power) : GameObject(image::Texture(render::Colour(0))), Position{pos}, Colour{col}, Power{power} {}

void Light::Draw()
{
    // the shader's uniform interface looks like this lights[lightIndex].Parameter
    std::string idx = "lights["; // lights[ + LightIndex + ]
    idx += std::to_string(render::LightIdx);
    idx += "].";

    // pass the values to the shader
    GlobalShader.Set(idx + "Colour", Colour);
    GlobalShader.Set(idx + "Position", Position + glm::vec3(1));
    GlobalShader.Set(idx + "Power", Power);
}

std::string Light::Type()
{
    return "Point Light";
}

GameObject *Light::Translate(glm::vec3 offset)
{
    Position += offset; // translate in the offset
    return this;
}

GameObject *Light::Move(glm::vec3 position)
{
    Position = position; // set the old position to the new one
    return this;
}