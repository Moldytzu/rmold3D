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

// Player component
class Player : public mold::render::objects::Component
{
public:
    float Speed = 25.0f; // adjust accordingly

    static void TestFunc(mold::render::objects::Component *self)
    {
        mold::log::Info("Hello from test func; \"tds\" is equals to " + std::to_string(GetAny(self->Public["tds"],int)));
    }

    void Tick() override
    {
        if (mold::input::GetKey(GLFW_KEY_UP))
            mold::render::camera::Rotate(glm::vec3(0, 0, 1), Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_DOWN))
            mold::render::camera::Rotate(glm::vec3(0, 0, -1), Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_LEFT))
            mold::render::camera::Rotate(glm::vec3(-1, 0, 0), Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_RIGHT))
            mold::render::camera::Rotate(glm::vec3(1, 0, 0), Speed * 2 * mold::time::DeltaTime);

        if (mold::input::GetKey('W'))
            mold::render::camera::Translate(glm::vec3(0, 0, 1), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('S'))
            mold::render::camera::Translate(glm::vec3(0, 0, -1), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('A'))
            mold::render::camera::Translate(glm::vec3(-1, 0, 0), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('D'))
            mold::render::camera::Translate(glm::vec3(1, 0, 0), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('I'))
            mold::render::camera::Translate(glm::vec3(0, 1, 0), Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('K'))
            mold::render::camera::Translate(glm::vec3(0, -1, 0), Speed * mold::time::DeltaTime / 10);

        mold::log::Info(std::to_string(mold::GlobalInputManager.Get("Up")));
    }

    void Start() override
    {
        mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Locked; // set proper cursor locking mode
        
        // set some public variables
        Public["test"] = 534;
        Public["abc"] = std::string("ab");
        Expose(TestFunc,"func"); // expose TestFunc as func

        // read those
        mold::log::Info("In \"test\" there is " + std::to_string(GetAny(Public["test"],int)));
        mold::log::Info("In \"abc\" there is " + GetAny(Public["abc"],std::string));
    }

    void Handle(mold::EventType event) override
    {
        if (event == mold::EventType::Mouse && mold::input::GlobalCursorLockMode == mold::CursorLockingMode::Locked) // update mouse
        {
            mold::render::camera::Yaw += mold::input::GlobalCursorAxis.x * mold::settings::MouseSensibility * mold::time::DeltaTime;
            mold::render::camera::Pitch += mold::input::GlobalCursorAxis.y * mold::settings::MouseSensibility * mold::time::DeltaTime;
        }
    }
};

class Game : public mold::Application
{
public:
    Game() : mold::Application(1280, 720) // 720p
    {
        // ground
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Plane(mold::render::image::Texture(mold::render::Colour(255, 0, 255))))->Scale(7.5f, 1.0f, 7.5f);

        // textured cube with face
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")), "Simple Cube")->Move(0, 1.0f, -1.0f)->Translate(0, 1.0f, -1.0f)->Scale(2.0f, 1.0f, 1.0f)->Rotate(glm::vec3(1), 75)->Opacity = 0.5f;

        // Instantiate some cubes
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255, 0, 0))))->Move(1.0f, 1.0f, 2.0f);
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0, 255, 0))))->Move(-1.0f, 0.5f, -2.0f);
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0, 0, 255))))->Move(1.0f, 0.0f, 2.0f);
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255))));

        // Set up skybox
        mold::GlobalSkybox = mold::render::Skybox("up.bmp","side.bmp","down.bmp");

        // Instantiate an empty gameobject as player
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Empty(), "Player")->AttachComponent("PlayerController", new Player)->Get("PlayerController")->Public["tds"] = 1024;
        mold::GlobalGameObjects.Get("Player")->Get("PlayerController")->CallFunc("func");

        // Instantiate a light
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Light(glm::vec3(-1,0,-1),glm::vec3(1),5),"Light");

        // Set up mappings
        mold::GlobalInputManager.Map("Up",GLFW_KEY_W)->Map("Down",GLFW_KEY_S)->Map("Left",GLFW_KEY_A)->Map("Right",GLFW_KEY_D);
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
            mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Locked;

        mold::GlobalGameObjects.Get("Light")->Move(mold::render::camera::Position);
    }

    void OnMouseInput() override
    {
        mold::settings::FOV -= mold::input::GlobalScrollAxis;
    }

    std::string Name() override
    {
        return "Example Game";
    }
};

mold::Application *mold::BuildApplication()
{
    return new Game();
}