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

void handleScroll(GLFWwindow *, double, double);

// glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    GlobalEventSystem.CallEvent(EventType::Resize); // call resize event
    settings::WindowHeight = height;                // set new window height and width
    settings::WindowWidth = height;
    glViewport(0, 0, width, height);                                   // set new viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // reset orthography
}

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
    glfwSetFramebufferSizeCallback(GlobalWindow, onResize);
    glfwSetScrollCallback(GlobalWindow, handleScroll);

    // set up gl
    glViewport(0, 0, width, height);                                   // set viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // set orthographic projection
    glEnable(GL_DEPTH_TEST);                                           // depth testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                 // substract alpha channel for transparency

    GlobalShader.AttachSource(render::VertexShaderSource, GL_VERTEX_SHADER);     // attach vertex shader
    GlobalShader.AttachSource(render::FragmentShaderSource, GL_FRAGMENT_SHADER); // attach fragment shader
    GlobalShader.Recompile();                                                    // compile it
    GlobalShader.Bind();
}

// mouse handling
double lastX;
double lastY;

void handleScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    input::GlobalScrollAxis = yoffset; // the scroll wheel in glfw works as a mouse where the yoffset is the cursor axis itself
}

void handleMouse()
{
    // handle mouse input
    double xpos, ypos;
    glfwGetCursorPos(GlobalWindow, &xpos, &ypos); // get cursor position

    // wrap cursor around window borders
    if (input::GlobalCursorLockMode == CursorLockingMode::Wrapped)
    {
        if (xpos > settings::WindowWidth)
        {
            glfwSetCursorPos(GlobalWindow, 10, ypos);
            lastX = 10;
            return;
        }

        if (xpos < 0)
        {
            glfwSetCursorPos(GlobalWindow, settings::WindowWidth - 10, ypos);
            lastX = settings::WindowWidth - 10;
            return;
        }

        if (ypos > settings::WindowHeight)
        {
            glfwSetCursorPos(GlobalWindow, xpos, 10);
            lastY = 10;
            return;
        }

        if (ypos < 0)
        {
            glfwSetCursorPos(GlobalWindow, xpos, settings::WindowHeight - 10);
            lastY = settings::WindowHeight - 10;
            return;
        }
    }

    // set axis data
    input::GlobalCursorAxis.x = xpos - lastX;
    input::GlobalCursorAxis.y = lastY - ypos;

    input::GlobalCursorPos.x = xpos;
    input::GlobalCursorPos.y = ypos;

    // call event if we've got changes
    if ((input::GlobalCursorAxis.x != 0 && input::GlobalCursorAxis.y != 0) || input::GlobalScrollAxis != 0)
        GlobalEventSystem.CallEvent(EventType::Mouse);

    // update last values
    lastX = xpos;
    lastY = ypos;
    input::GlobalScrollAxis = 0; // reset scroll value
}

float oldYaw, oldPitch;
glm::vec3 oldDirection;
int oldFPS;
CursorLockingMode oldLockingMode;
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

        glClearColor(0, 0, 0, 0);                           // black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and the depth buffer

        // enable/disable transparency and msaa depending on the settings
        if (settings::MSAAEnabled)
            glEnable(GL_MULTISAMPLE);
        else
            glDisable(GL_MULTISAMPLE);

        if (settings::TransparencyEnabled)
            glEnable(GL_BLEND);
        else
            glDisable(GL_BLEND);

        // update window title (Rewritten mold 3D @ ?? FPS)
        if (oldFPS != (int)(1 / time::DeltaTime))
        {
            std::string wtitle = "Rewritten mold 3D @ " + std::to_string((int)(1 / time::DeltaTime)) + " FPS";
            glfwSetWindowTitle(GlobalWindow, wtitle.c_str());
            oldFPS = (int)(1 / time::DeltaTime);
        }

        // handle cursor locking mode
        if (oldLockingMode != input::GlobalCursorLockMode)
        {
            switch (input::GlobalCursorLockMode)
            {
            case CursorLockingMode::Normal:
                glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                break;
            case CursorLockingMode::Locked:
                glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
            case CursorLockingMode::Centred:
                glfwSetCursorPos(GlobalWindow, settings::WindowWidth / 2, settings::WindowHeight / 2);
                glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                break;
            default:
                glfwSetInputMode(GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
                break;
            }
            oldLockingMode = input::GlobalCursorLockMode; // save old value
        }

        // do mouse handling
        handleMouse();

        // update camera front
        if (render::camera::Yaw != oldYaw || render::camera::Pitch != oldPitch) // update only when the values change so we don't do cos and sin on every tick
        {
            // clamp the value of pitch
            render::camera::Pitch = glm::clamp(render::camera::Pitch, -89.0f, 89.0f);

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
            render::camera::Projection = glm::perspective(glm::radians(settings::FOV), settings::WindowWidth / settings::WindowHeight, 0.1f, 100.0f);

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
        if (settings::LightingEnabled) // bind sun if the lighting is enabled
            GlobalSun.Bind();

        // draw game objects
        for (auto const &[name, ptr] : GlobalGameObjects.Get())
        {
            if (!ptr->Enabled)
                continue;

            if (render::camera::InView(ptr->GetPosition()) && ptr->Type() != "Empty") // draw if the object is in view and if it isn't an empty gameobject
            {
                ptr->Bind(); // bind vabo, texture and matrices
                ptr->Draw(); // do drawing
            }
            ptr->TickComponents(); // tick it's components
        }

        glFlush(); // flush pipeline

        glfwSwapBuffers(GlobalWindow); // swap buffers
        glfwPollEvents();              // poll events
    }
}