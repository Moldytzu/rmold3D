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

// threading
std::vector<std::thread> threads; // threads vector

// clean up
void mold::Destroy()
{
    Events::CallEvent(EventType::Exit); // call exit event

    for (auto const &[name, ptr] : GlobalGameObjects.Get())
        delete ptr; // delete gameobject

    render::objects::GameObject tmp;
    for (auto const &[name, ptr] : tmp.GetComponents())
        delete ptr; // delete all components

    delete GlobalApplication; // deconstuct application

    GlobalShader.Deallocate(); // deallocate shader
    GlobalSkybox.Deallocate(); // deallocate skybox

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

    // set up all signals
    for (int i = SIGRTMIN; i <= SIGRTMAX; i++)
        signal(i, mold::HandleSignal);

    // init renderer
    mold::render::Init(width, height);

    // load settings
    mold::settings::LoadFromFile("mold.cfg");

    // set up sun
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Light(glm::vec3(1, 1, 1), glm::vec3(1), 50), "Sun");

    // set up default skybox
    mold::GlobalSkybox = render::Skybox(render::Colour(0));
}

int oldFPS;
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

        // clear threads
        threads.clear();

        // call tick
        threads.push_back(std::thread(&Events::CallEvent, EventType::Tick));

        // update window title (Rewritten mold 3D @ ?? FPS)
        int fps = (int)(1 / time::DeltaTime);
        if (oldFPS != fps)
        {
            std::string wtitle = "Rewritten mold 3D @ " + std::to_string(fps) + " FPS";
            glfwSetWindowTitle(GlobalWindow, wtitle.c_str());
            oldFPS = fps;
        }

        // do cursor handling
        threads.push_back(std::thread(&mold::input::HandleCursor));

        // do mouse handling
        threads.push_back(std::thread(&mold::input::HandleMouse));

        // handle camera
        threads.push_back(std::thread(&mold::render::camera::Handle));

        // wait for the threads
        std::for_each(threads.begin(), threads.end(), [](std::thread &t)
                      { t.join(); });

        // do the rendering
        render::Render();
    }
}