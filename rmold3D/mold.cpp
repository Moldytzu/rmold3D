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

using namespace mold;

// clean up
void mold::Destroy()
{
    GlobalEventSystem.CallEvent(EventType::Exit); // call exit event

    for (auto const &[name, ptr] : GlobalGameObjects.Get())
        delete ptr; // delete gameobject

    render::objects::GameObject tmp;
    for (auto const &[name, ptr] : tmp.GetComponents())
        delete ptr; // delete all components

    delete GlobalApplication; // deconstuct application

    GlobalShader.Deallocate(); // deallocate shader
    GlobalSkybox.Deallocate(); // deallocate skybox

    glfwTerminate(); // terminate glfw

    exit(0); // exit
}

void mold::Init(uint width, uint height)
{
    settings::WindowHeight = height;
    settings::WindowWidth = width;

    log::Info("Starting Rewritten mold 3D");

    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core
    glfwWindowHint(GLFW_SAMPLES, 4);                               // MSAA x4

    // set rng
    srand((uint64_t)glfwGetTime());

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

    // load settings
    mold::settings::LoadFromFile("mold.cfg");

    // set up sun
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Light(glm::vec3(1, 1, 1), glm::vec3(1), 50), "Sun");

    // set up default skybox
    mold::GlobalSkybox = render::Skybox(render::Colour(0));

    GlobalShader.AttachSource(render::VertexShaderSource, GL_VERTEX_SHADER);     // attach vertex shader
    GlobalShader.AttachSource(render::FragmentShaderSource, GL_FRAGMENT_SHADER); // attach fragment shader
    GlobalShader.Recompile();                                                    // compile it
    GlobalShader.Bind();
}

float oldYaw, oldPitch;
glm::vec3 oldDirection;
int oldFPS;
glm::vec3 oldPosition, oldFront;

void mold::Run()
{
    log::Info("Running " + GlobalApplication->Name());

    // run the game if the window is open
    while (!glfwWindowShouldClose(GlobalWindow))
    {
        // calculate delta time
        float currentFrame = glfwGetTime();
        time::DeltaTime = currentFrame - time::LastFrame;
        time::LastFrame = currentFrame;

        // call tick
        GlobalEventSystem.CallEvent(EventType::Tick);

        glClear(GL_DEPTH_BUFFER_BIT); // clear the depth buffer

        // update window title (Rewritten mold 3D @ ?? FPS)
        if (oldFPS != (int)(1 / time::DeltaTime))
        {
            std::string wtitle = "Rewritten mold 3D @ " + std::to_string((int)(1 / time::DeltaTime)) + " FPS";
            glfwSetWindowTitle(GlobalWindow, wtitle.c_str());
            oldFPS = (int)(1 / time::DeltaTime);
        }

        // handle cursor locking mode
        input::HandleCursor();

        // do mouse handling
        input::HandleMouse();

        // update camera front
        if (render::camera::Yaw != oldYaw || render::camera::Pitch != oldPitch) // update only when the values change so we don't do cos and sin on every tick
        {
            // clamp the value of pitch
            render::camera::Pitch = glm::clamp(render::camera::Pitch, -89.9f, 89.9f);

            oldDirection.x = cos(glm::radians(render::camera::Yaw)) * cos(glm::radians(render::camera::Pitch));
            oldDirection.y = sin(glm::radians(render::camera::Pitch));
            oldDirection.z = sin(glm::radians(render::camera::Yaw)) * cos(glm::radians(render::camera::Pitch));
            render::camera::Front = glm::normalize(oldDirection);

            oldYaw = render::camera::Yaw; // save the values
            oldPitch = render::camera::Pitch;
        }

        // update view and projection
        if (render::camera::Position != oldPosition || render::camera::Front != oldFront)
        {
            render::camera::View = glm::lookAt(render::camera::Position, render::camera::Position + render::camera::Front, render::camera::Up);
            render::camera::Projection = glm::perspective(math::Vfov(settings::FOV, (settings::WindowWidth / settings::WindowHeight)), settings::WindowWidth / settings::WindowHeight, 0.1f, 100.0f);

            oldPosition = render::camera::Position; // save the values
            oldFront = render::camera::Front;
        }

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
}