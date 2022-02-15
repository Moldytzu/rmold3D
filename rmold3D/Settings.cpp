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

using namespace mold::settings;

void mold::settings::Update()
{
    // enable/disable transparency and msaa depending on the settings
    if (settings::MSAAEnabled)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

    if (settings::TransparencyEnabled)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);
}

void mold::settings::LoadFromFile(std::string filename)
{
    mold::settings::Update(); // flush default settings

    std::ifstream stream(filename); // create stream

    if (!stream.good()) // fails if file doesn't exist
    {
        log::Warn("Failed to load settings from " + filename + ". Using default settings.");
        return;
    }

    std::string line;
    while (std::getline(stream, line)) // read line by line
    {
        std::istringstream strm(line);
        std::string key;
        if (std::getline(strm, key, '='))
        {
            strm >> std::ws;
            std::string value;
            if (std::getline(strm, value))
            {
                // parse key and value
                if (key.back() == ' ') // remove last space
                    key.pop_back();
                for (auto &c : key)
                    c = std::tolower(c);

                if (key == "fov")
                    settings::FOV = std::stof(value);
                else if (key == "mousesensibility")
                    settings::MouseSensibility = std::stof(value);
                else if (key == "fogenabled")
                    settings::FogEnabled = (bool)((int)std::stoi(value));
                else if (key == "fogdensity")
                    settings::FogDensity = std::stof(value);
                else if (key == "fogcolour.r")
                    settings::FogColour.r = std::stoi(value);
                else if (key == "fogcolour.g")
                    settings::FogColour.g = std::stoi(value);
                else if (key == "fogcolour.b")
                    settings::FogColour.b = std::stoi(value);
                else if (key == "fogautodensity")
                    settings::FogAutoDensity = (bool)((int)std::stoi(value));
                else if (key == "lightingenabled")
                    settings::LightingEnabled = (bool)((int)std::stoi(value));
                else if (key == "lightingambient")
                    settings::LightingAmbient = std::stof(value);
                else if (key == "skyboxdistance")
                    settings::SkyboxDistance = std::stoi(value);
                else if (key == "msaaenabled")
                    settings::MSAAEnabled = (bool)((int)std::stoi(value));
                else if (key == "transparencyenabled")
                    settings::TransparencyEnabled = (bool)((int)std::stoi(value));
                else
                    mold::log::Warn("Unknown setting " + key);
            }
        }
    }

    mold::settings::Update(); // flush settings
}

void mold::settings::SaveToFile(std::string filename)
{
}