#include <rmold3D/mold.h>

/*
Cubes stress test for rmold3D

Started with: 
- 2000 cubes: 20 FPS with stutters; 20 seconds to start up; 1.4 GB RAM (mouse f-ed up)
- 200 cubes: 55-60 FPS; 1.5 seconds to start up (mouse sped up)
- 20 cubes: 55-60 FPS; 0.4 seconds to start up
- 2 cubes: 55-60 FPS; 0.25 seconds to start up

Right now we have:
- 2000 cubes: 60 FPS; 1 second to start up with 65 MB RAM
- 10000 cubes: 47 FPS; 19 seconds to start up with 71 MB RAM

*/

#define CUBES 200

// Player component
class Player : public mold::render::objects::Component
{
public:
    float Speed = 25.0f; // adjust accordingly
    void Tick()
    {
        if (mold::input::GetKey(GLFW_KEY_UP))
            mold::render::camera::Rotate(mold::render::CameraDirection::Forward, Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_DOWN))
            mold::render::camera::Rotate(mold::render::CameraDirection::Backwards, Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_LEFT))
            mold::render::camera::Rotate(mold::render::CameraDirection::Left, Speed * 2 * mold::time::DeltaTime);
        if (mold::input::GetKey(GLFW_KEY_RIGHT))
            mold::render::camera::Rotate(mold::render::CameraDirection::Right, Speed * 2 * mold::time::DeltaTime);

        if (mold::input::GetKey('W'))
            mold::render::camera::Translate(mold::render::CameraDirection::Forward, Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('S'))
            mold::render::camera::Translate(mold::render::CameraDirection::Backwards, Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('A'))
            mold::render::camera::Translate(mold::render::CameraDirection::Left, Speed * mold::time::DeltaTime / 10);
        if (mold::input::GetKey('D'))
            mold::render::camera::Translate(mold::render::CameraDirection::Right, Speed * mold::time::DeltaTime / 10);
    }

    void Start(mold::render::objects::GameObject *parent)
    {
        mold::render::objects::Component::Start(parent);                      // call super
        mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Wrapped; // set proper cursor locking mode
    }

    void Handle(mold::EventType event)
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
};

mold::Application *mold::BuildApplication()
{
    return new Game();
}