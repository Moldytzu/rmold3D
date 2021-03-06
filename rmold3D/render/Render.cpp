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
    profiler::glVertices += count;        // add the vertices count to the global count
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

void mold::render::Init(uint width, uint height)
{
    glfwSetErrorCallback(&mold::render::OnError);
    glfwInit(); // initialize glfw

    log::Info("glfw " + std::string(glfwGetVersionString()));

    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3+ Core
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
    glfwSetScrollCallback(GlobalWindow, mold::input::OnScroll);
    glfwSetCursorPosCallback(GlobalWindow, mold::input::OnMouse);

    // set up gl
    glViewport(0, 0, width, height);                                   // set viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // set orthographic projection
    glEnable(GL_DEPTH_TEST);                                           // depth testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                 // substract alpha channel for transparency

    // compile shader
    GlobalShader.AttachSource(render::VertexShaderSource, GL_VERTEX_SHADER);     // attach vertex shader
    GlobalShader.AttachSource(render::FragmentShaderSource, GL_FRAGMENT_SHADER); // attach fragment shader
    GlobalShader.Recompile();                                                    // compile it
    GlobalShader.Bind();
}

void mold::render::Render()
{
    profiler::glVertices = 0; // reset vertices count

    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT); // clear the depth buffer and colour buffer

    // ensure that we use shader
    GlobalShader.Bind();

    // give the shader our view and projection
    GlobalShader.Set("view", render::camera::View);
    GlobalShader.Set("projection", render::camera::Projection);

    // draw stuff
    events::CallEvent(EventType::Redraw);

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
        render::LightIdx = 0;                    // reset light index
        events::CallEvent(EventType::LightTick); // tick light
    }

    // call tick
    events::CallEvent(EventType::Tick);

    for (auto const &[name, ptr] : GlobalGameObjects.Get())
    {
        // if (!ptr->Enabled)
        //     continue;

        ptr->TickComponents(); // tick it's components
    }

    render::HandleErrors(); // detect gl errors

    glFlush(); // flush pipeline

    glfwSwapBuffers(GlobalWindow); // swap buffers
    glfwPollEvents();              // poll events
}