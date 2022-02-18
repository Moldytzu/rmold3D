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

const float fogDensityPerSkyboxSize = 2.5f;

void mold::render::HandleFog()
{
    // set fog parameters
    GlobalShader.Set("fogEnabled", settings::FogEnabled);

    // set density and colour only if the fog is enabled
    if (!settings::FogEnabled)
        return;

    GlobalShader.Set("fogDensity", settings::FogDensity);
    GlobalShader.Set("fogColour", settings::FogColour);

    // calculate fog density
    if (!settings::FogAutoDensity)
        return;

    settings::FogDensity = fogDensityPerSkyboxSize / settings::SkyboxDistance; // basic equation to get fog density based on the skybox size
}