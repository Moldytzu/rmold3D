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
    mold::log::Debug("Updating settings");

    // enable/disable transparency and msaa depending on the settings
    if (settings::MSAAEnabled)
        glEnable(GL_MULTISAMPLE);
    else
        glDisable(GL_MULTISAMPLE);

    if (settings::TransparencyEnabled)
        glEnable(GL_BLEND);
    else
        glDisable(GL_BLEND);

    if (settings::DebugRenderer)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    // enable/disable swap interval
    if(settings::VSYNC)
        glfwSwapInterval(1); // swap interval
    else
        glfwSwapInterval(0); // no swap interval 

    // switch on/off fullscreen
    if(settings::FullScreen)
    {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode *mode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor(GlobalWindow,monitor,0,0,mode->width,mode->height,mode->refreshRate);
    }
    else
        glfwWindowHint(GLFW_AUTO_ICONIFY , GLFW_TRUE); // restore resolution

    // clamp skybox distance to not get funky results
    settings::SkyboxDistance = glm::clamp(settings::SkyboxDistance, 0.0f, 100.0f);
}

void mold::settings::LoadFromFile(std::string filename)
{
    mold::settings::Update(); // flush default settings

    mold::log::Debug("Trying to loading settings from " + filename);

    std::ifstream stream(filename); // create stream

    if (!stream.good()) // fails if file doesn't exist
    {
        log::Warn("Failed to load settings from " + filename + ". Using default settings.");
        mold::settings::SaveToFile(filename);
        return;
    }

    std::string line;
    while (std::getline(stream, line)) // read line by line
    {
        std::istringstream strm(line);
        std::string key;
        if (std::getline(strm, key, '='))
        {
            strm >> std::ws; // remove white space
            std::string value;
            if (std::getline(strm, value))
            {
                // parse key and value
                if (key.back() == ' ') // remove last white space
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
                else if (key == "gammacorrection")
                    settings::GammaCorrection = (bool)((int)std::stoi(value));
                else if (key == "debugrenderer")
                    settings::DebugRenderer = (bool)((int)std::stoi(value));
                else if (key == "debug")
                    settings::Debug = (bool)((int)std::stoi(value));
                else if (key == "vsync")
                    settings::VSYNC = (bool)((int)std::stoi(value));
                else if (key == "fullscreen")
                    settings::FullScreen = (bool)((int)std::stoi(value));
                else if (key == "gamma")
                    settings::Gamma = std::stof(value);
                else if (key == "aspectratio")
                    settings::AspectRatio = std::stof(value);
                else if (key == "framelimit")
                    settings::FrameLimit = std::stof(value);
                else
                    mold::log::Warn("Unknown setting " + key);
            }
        }
    }

    stream.close(); // close stream

    mold::settings::Update(); // flush settings
}

void mold::settings::SaveToFile(std::string filename)
{
    std::ofstream stream(filename); // create stream

    if (!stream.good()) // fails if file doesn't exist
    {
        log::Warn("Failed to save settings to " + filename + ". Aborting.");
        return;
    }

    stream << "Config file generated by rmold3D" << std::endl;
    stream << "MouseSensibility = " << settings::MouseSensibility << std::endl;
    stream << "FOV = " << settings::FOV << std::endl;
    stream << "FogEnabled = " << settings::FogEnabled << std::endl;
    stream << "FogDensity = " << settings::FogDensity << std::endl;
    stream << "FogColour.r = " << settings::FogColour.r << std::endl;
    stream << "FogColour.g = " << settings::FogColour.g << std::endl;
    stream << "FogColour.b = " << settings::FogColour.b << std::endl;
    stream << "FogAutoDensity = " << settings::FogAutoDensity << std::endl;
    stream << "LightingEnabled = " << settings::LightingEnabled << std::endl;
    stream << "LightingAmbient = " << settings::LightingAmbient << std::endl;
    stream << "SkyboxDistance = " << settings::SkyboxDistance << std::endl;
    stream << "MSAAEnabled = " << settings::MSAAEnabled << std::endl;
    stream << "VSYNC = " << settings::VSYNC << std::endl;
    stream << "FullScreen = " << settings::FullScreen << std::endl;
    stream << "TransparencyEnabled = " << settings::TransparencyEnabled << std::endl;
    stream << "GammaCorrection = " << settings::GammaCorrection << std::endl;
    stream << "Gamma = " << settings::Gamma << std::endl;
    stream << "DebugRenderer = " << settings::DebugRenderer << std::endl;
    stream << "Debug = " << settings::Debug << std::endl;
    stream << "AspectRatio = " << settings::AspectRatio << std::endl;
    stream << "FrameLimit = " << settings::FrameLimit << std::endl;
}