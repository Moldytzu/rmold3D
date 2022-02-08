#include <rmold3D/mold.h>

/*remaining features to add:
- lighting
- internal console
- internal profiler
*/

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
    Game() : mold::Application(1280,720) // 720p
    {
        // ground
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Plane(mold::render::image::Texture(mold::render::Colour(255, 0, 255))))->Scale(glm::vec3(7.5f, 1.0f, 7.5f));

        // textured cube with face
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")), "Simple Cube")->Move(glm::vec3(0, 1.0f, -1.0f))->Translate(glm::vec3(0, 1.0f, -1.0f))->Scale(glm::vec3(2.0f, 1.0f, 1.0f))->Rotate(glm::vec3(1), 75)->Opacity = 0.5f;

        //Instantiate some cubes
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255, 0, 0))))->Move(glm::vec3(1.0f, 1.0f, 2.0f));
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0, 255, 0))))->Move(glm::vec3(-1.0f, 0.5f, -2.0f));
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0, 0, 255))))->Move(glm::vec3(1.0f, 0.0f, 2.0f));
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255))));

        //Instantiate an empty gameobject as player
        mold::GlobalGameObjects.Instantiate(new mold::render::objects::Empty(), "Player")->AttachComponent("PlayerController", new Player);
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