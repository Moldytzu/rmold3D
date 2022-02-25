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
        mold::render::camera::Rotate(glm::vec3(0, 0, 1), mold::GlobalInputManager.Get("Up") * Speed * 2 * mold::time::DeltaTime);
        mold::render::camera::Rotate(glm::vec3(0, 0, -1), mold::GlobalInputManager.Get("Down") * Speed * 2 * mold::time::DeltaTime);
        mold::render::camera::Rotate(glm::vec3(-1, 0, 0), mold::GlobalInputManager.Get("Left") * Speed * 2 * mold::time::DeltaTime);
        mold::render::camera::Rotate(glm::vec3(1, 0, 0), mold::GlobalInputManager.Get("Right") * Speed * 2 * mold::time::DeltaTime);

        mold::render::camera::Translate(glm::vec3(0, 0, 1), mold::GlobalInputManager.Get("W") * Speed * mold::time::DeltaTime / 10);
        mold::render::camera::Translate(glm::vec3(0, 0, -1), mold::GlobalInputManager.Get("S") * Speed * mold::time::DeltaTime / 10);
        mold::render::camera::Translate(glm::vec3(-1, 0, 0), mold::GlobalInputManager.Get("A") * Speed * mold::time::DeltaTime / 10);
        mold::render::camera::Translate(glm::vec3(1, 0, 0), mold::GlobalInputManager.Get("D") * Speed * mold::time::DeltaTime / 10);

        mold::render::camera::Translate(glm::vec3(0, 1, 0), mold::GlobalInputManager.Get("I") * Speed * mold::time::DeltaTime / 10);
        mold::render::camera::Translate(glm::vec3(0, -1, 0), mold::GlobalInputManager.Get("K") * Speed * mold::time::DeltaTime / 10);
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
            mold::render::camera::Yaw += mold::GlobalInputManager.Get("Horizontal") * mold::settings::MouseSensibility * mold::time::DeltaTime;
            mold::render::camera::Pitch += mold::GlobalInputManager.Get("Vertical") * mold::settings::MouseSensibility * mold::time::DeltaTime;
        }
    }
};

class Game : public mold::Application
{
public:
    Game() : mold::Application(1280, 720) // 720p
    {
        // ground
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Plane())->Scale(7.5f, 1.0f, 7.5f)->Get("Renderer")->Public["Texture"] = mold::render::image::Texture(mold::render::Colour(255, 0, 255)); // scale then attach plane renderer

        // textured cube with face
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(), "Simple Cube")->Move(0, 1.0f, -1.0f)->Translate(0, 1.0f, -1.0f)->Scale(2.0f, 1.0f, 1.0f)->Rotate(glm::vec3(1), 75)->Get("Renderer")->Public["Texture"] = mold::render::image::Texture("texture.bmp");

        // Instantiate some cubes
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube())->Move(1.0f, 1.0f, 2.0f)->Get("Renderer")->Public["Texture"] = mold::render::image::Texture(mold::render::Colour(255, 0, 0));
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube())->Move(-1.0f, 0.5f, -2.0f)->Get("Renderer")->Public["Texture"] = mold::render::image::Texture(mold::render::Colour(0, 255, 0));
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube())->Move(-2.0f, 0.5f, -2.0f)->Get("Renderer")->Public["Texture"] = mold::render::image::Texture(mold::render::Colour(0, 0, 255));

        // Set up skybox
        mold::GlobalSkybox = mold::render::Skybox("up.bmp","side.bmp","down.bmp");

        // Instantiate an empty gameobject as player
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Empty(), "Player")->AttachComponent("PlayerController", new Player)->Get("PlayerController")->Public["tds"] = 1024;
        mold::GlobalGameObjects.Get("Player")->Get("PlayerController")->CallFunc("func");

        // Test
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(), "Test");

        // Instantiate a light
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Light(glm::vec3(-1,0,-1),glm::vec3(1),5),"Light");
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
        if (mold::GlobalInputManager.Get("Escape"))
            mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Normal;

        if (mold::GlobalInputManager.Get("F1"))
            mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Locked;

        mold::GlobalGameObjects.Get("Light")->Move(mold::render::camera::Position);
    }

    void OnMouseInput() override
    {
        mold::settings::FOV -= mold::GlobalInputManager.Get("Scroll");
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