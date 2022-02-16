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

mold::render::Colour::Colour(uint8_t r, uint8_t g, uint8_t b) : R{r}, G{g}, B{b} {} // set RGB values individually
mold::render::Colour::Colour(uint8_t rgb) : R{rgb}, G{rgb}, B{rgb} {}               // set RGB to the same value

void mold::render::DrawTriangles(uint count)
{
    glDrawArrays(GL_TRIANGLES, 0, count); // draw triangles
}

void mold::render::HandleErrors()
{
    // check for opengl errors
    GLenum error = glGetError();
    switch (error)
    {
    case GL_INVALID_ENUM:
        log::Error("GL_INVALID_ENUM");
        break;
    case GL_INVALID_VALUE:
        log::Error("GL_INVALID_VALUE");
        break;
    case GL_INVALID_OPERATION:
        log::Error("GL_INVALID_OPERATION");
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        log::Error("GL_INVALID_FRAMEBUFFER_OPERATION");
        break;
    case GL_OUT_OF_MEMORY: // crash if we're out of vram
        log::Fatal("GL_OUT_OF_MEMORY");
        break;
    default:
        break;
    }
}

void mold::render::Init(uint width,uint height)
{
    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core
    glfwWindowHint(GLFW_SAMPLES, 4);                               // MSAA x4

    GlobalWindow = glfwCreateWindow(width, height, "Rewritten mold 3D", NULL, NULL); // create window
    if (GlobalWindow == NULL)                                                        // exit if the window couldn't be created
        log::Fatal("Couldn't create glfw window!");

#ifdef GLFW_RAW_MOUSE_MOTION
    if (glfwRawMouseMotionSupported()) // enable raw mouse motion if supported
        glfwSetInputMode(GlobalWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

    glfwMakeContextCurrent(GlobalWindow); // create opengl context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
        log::Fatal("Couldn't load glad!");

    log::Info("Rendering OpenGL " + std::string((const char *)glGetString(GL_VERSION)) + " on a " + std::string((const char *)glGetString(GL_VENDOR)) + " " + std::string((const char *)glGetString(GL_RENDERER))); // display opengl information

    // set up glfw callbacks
    glfwSetFramebufferSizeCallback(GlobalWindow, render::OnResize);
    glfwSetScrollCallback(GlobalWindow, render::OnScroll);

    // set up gl
    glViewport(0, 0, width, height);                                   // set viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // set orthographic projection
    glEnable(GL_DEPTH_TEST);                                           // depth testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                 // substract alpha channel for transparency
}

void mold::render::Render()
{
    // ensure that we use shader
    GlobalShader.Bind();

    // give the shader our view and projection
    GlobalShader.Set("view", render::camera::View);
    GlobalShader.Set("projection", render::camera::Projection);

    // draw stuff
    GlobalEventSystem.CallEvent(EventType::Redraw);

    // draw skybox
    GlobalSkybox.Bind();

    // handle fog
    render::HandleFog();

    // set lighting parameters
    GlobalShader.Set("lightingEnabled", settings::LightingEnabled);

    // set gamma
    GlobalShader.Set("gammaCorectionEnabled", settings::GammaCorrection);
    GlobalShader.Set("gamma", mold::settings::Gamma);

    // draw lights
    if (settings::LightingEnabled) // draw lights only if it's enabled
    {
        render::LightIdx = 0; // reset light index
        for (auto const &[name, ptr] : GlobalGameObjects.Get())
        {
            if (!ptr->Enabled)
                continue;

            if (render::camera::InView(ptr->GetPosition()) && ptr->Type() == "Point Light") // draw only lights
            {
                ptr->Draw();
                render::LightIdx++;
            }
        }
    }

    // draw game objects
    for (auto const &[name, ptr] : GlobalGameObjects.Get())
    {
        if (!ptr->Enabled)
            continue;

        if (render::camera::InView(ptr->GetPosition()) && ptr->Type() != "Empty" && ptr->Type() != "Point Light") // draw if the object is in view and if it isn't an empty gameobject or a light
        {
            ptr->Bind(); // bind vabo, texture and matrices
            ptr->Draw(); // do drawing
        }
        ptr->TickComponents(); // tick it's components
    }

    // detect gl errors
    render::HandleErrors();

    glFlush(); // flush pipeline

    glfwSwapBuffers(GlobalWindow); // swap buffers
    glfwPollEvents();              // poll events
}