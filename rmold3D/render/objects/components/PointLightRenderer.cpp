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

#include "../../../mold.h"

using namespace mold::render::objects;

void PointLightRenderer::Start()
{
    Public["Colour"] = glm::vec3(1);
    Public["Power"] = 1.0f;
}

void PointLightRenderer::Handle(mold::EventType event)
{
    if(event != mold::EventType::LightTick) return; // we handle only light ticks
    
    // the shader's uniform interface looks like this lights[lightIndex].Parameter
    std::string idx = "lights["; // lights[ + LightIndex + ]
    idx += std::to_string(render::LightIdx);
    idx += "].";

    // pass the values to the shader
    GlobalShader.Set(idx + "Colour", GetAny(Public["Colour"],glm::vec3));
    GlobalShader.Set(idx + "Position", Parent->GetPosition() + glm::vec3(1));
    GlobalShader.Set(idx + "Power", GetAny(Public["Power"],float));
    
    render::LightIdx++; // increase light index
}