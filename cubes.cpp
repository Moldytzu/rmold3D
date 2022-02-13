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

#include <rmold3D/mold.h>

/*
Cubes stress test for rmold3D

Started with: 
- 2000 cubes: 20 FPS with stutters; 20 seconds to start up; consuming 1.4 GB RAM (mouse f-ed up)
- 200 cubes: 55-60 FPS; 1.5 seconds to start up (mouse sped up)
- 20 cubes: 55-60 FPS; 0.4 seconds to start up
- 2 cubes: 55-60 FPS; 0.25 seconds to start up

Right now we have:
- 2000 cubes: 60 FPS; 1.5 second to start up; consuming 64 MB RAM
- 5000 cubes: 40 FPS; 9 seconds to start up
- 10000 cubes: 20 FPS; 46 seconds to start up; consuming 96 MB RAM

*/

#define CUBES 5000

// Player component
class Player : public mold::render::objects::Component
{
public:
    float Speed = 25.0f; // adjust accordingly
    void Tick() override
    {
        if (mold::input::GetKey(GLFW_KEY_UP))
            mold::render::camera::Rotate(glm::vec3(0,0,1), Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_DOWN))
            mold::render::camera::Rotate(glm::vec3(0,0,-1), Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_LEFT))
            mold::render::camera::Rotate(glm::vec3(-1,0,0), Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_RIGHT))
            mold::render::camera::Rotate(glm::vec3(1,0,0), Speed * 2 * mold::time::DeltaTime);

        if (mold::input::GetKey('W'))
            mold::render::camera::Translate(glm::vec3(0,0,1), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('S'))
            mold::render::camera::Translate(glm::vec3(0,0,-1), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('A'))
            mold::render::camera::Translate(glm::vec3(-1,0,0), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('D'))
            mold::render::camera::Translate(glm::vec3(1,0,0), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('I'))
            mold::render::camera::Translate(glm::vec3(0,1,0), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('K'))
            mold::render::camera::Translate(glm::vec3(0,-1,0), Speed * mold::time::DeltaTime / 10);
    }

    void Start() override
    {
        mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Wrapped; // set proper cursor locking mode
    }

    void Handle(mold::EventType event) override
    {
        if (event == mold::EventType::Mouse) // update mouse
        {
            mold::render::camera::Yaw += mold::input::GlobalCursorAxis.x * mold::settings::MouseSensibility * mold::time::DeltaTime;
            mold::render::camera::Pitch += mold::input::GlobalCursorAxis.y * mold::settings::MouseSensibility * mold::time::DeltaTime;
        }
    }
};

class Game : public mold::Application
{
public:
    Game() : mold::Application(1024,768)
    {
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")));

        //Instantiate many cubes
        int offset = 0;
        for (int i = 1; i < CUBES; i++)
        {
            mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")));
            mold::GlobalGameObjects.Get("Textured Cube " + std::to_string(i))->Translate(glm::vec3(offset++));
        }

        //Instantiate an empty gameobject as player
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::GameObject(mold::render::image::Texture(mold::render::Colour(0))), "Player");
        mold::GlobalGameObjects.Get("Player")->AttachComponent("PlayerController", new Player);

        mold::settings::FogColour = glm::vec4(0,0,0,1); // black
    }

    ~Game()
    {
        mold::log::Info("Goodbye!");
    }

    void OnResize() override
    {
        mold::log::Info("Resize!");
    }

    void Tick() override
    {
        if (mold::input::GetKey(GLFW_KEY_ESCAPE))
            mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Normal;

        if (mold::input::GetKey(GLFW_KEY_F1))
            mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Wrapped;
    }

    void OnMouseInput() override
    {
        mold::settings::FOV -= mold::input::GlobalScrollAxis;
    }

    std::string Name() override
    {
        return "Cubes Stress-test";
    }
};

mold::Application *mold::BuildApplication()
{
    return new Game();
}