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
    Events::CallEvent(EventType::Exit); // call exit event

    for (auto const &[name, ptr] : GlobalGameObjects.Get())
        delete ptr; // delete gameobject

    render::objects::GameObjectBase g;
    for (auto const &[name, ptr] : g.GetRaw())
        delete ptr; // delete all components

    delete GlobalApplication; // deconstuct application

    GlobalThreads.Reset(); // deallocate threads
    GlobalShader.Deallocate(); // deallocate shader
    GlobalSkybox.Deallocate(); // deallocate skybox

    glfwDestroyWindow(GlobalWindow); // destroy window

    glfwTerminate(); // terminate glfw

    settings::SaveToFile("mold.cfg"); // save settings

    exit(0); // exit
}

void mold::Init(uint width, uint height)
{
    settings::WindowHeight = height;
    settings::WindowWidth = width;

    log::Info("Starting Rewritten mold 3D");

    // set rng
    srand((uint64_t)glfwGetTime());

// set up all signals if we're in a posix enviroment
#ifdef SIGRTMIN
    for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
        signal(i, mold::HandleSignal);
#endif

    // init renderer
    mold::render::Init(width, height);

    // load settings
    mold::settings::LoadFromFile("mold.cfg");

    // set up sun
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::PointLight(), "Sun")->Get("Renderer")->Public["Power"] = 50.0f;
    mold::GlobalGameObjects.Get("Sun")->Move(glm::vec3(1)); // 1,1,1

    // set up default skybox
    mold::GlobalSkybox = render::Skybox(render::Colour(0));
}

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

        // calculate FPS
        time::FPS = (int)(1 / time::DeltaTime);

        if(settings::FrameLimit != 0) // 0 means unlimited
        {
            // calculate wait time
            double waitTime = settings::FrameLimit - time::DeltaTime + 1;
            if(waitTime > 0)
                std::this_thread::sleep_for(std::chrono::milliseconds((int)waitTime));
        }

        // reset threads
        GlobalThreads.Reset();

        // update window title (Rewritten mold 3D @ ?? FPS)
        std::string wtitle = "Rewritten mold 3D @ " + std::to_string(time::FPS) + " FPS";
        glfwSetWindowTitle(GlobalWindow, wtitle.c_str());

        // do cursor handling
        GlobalThreads.Add(new std::thread(&mold::input::HandleCursor));

        // handle camera
        GlobalThreads.Add(new std::thread(&mold::render::camera::Handle));

        // wait for the threads
        GlobalThreads.Wait();

        // do the rendering
        render::Render();
    }
}