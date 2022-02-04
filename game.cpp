#include <rmold3D/mold.h>

/*remaining features to add:
- lighting
- internal console
- internal profiler
*/

// mouse update event
void onMouse()
{
    mold::render::camera::Yaw += mold::input::GlobalCursorAxisX * mold::settings::MouseSensibility;
    mold::render::camera::Pitch += mold::input::GlobalCursorAxisY * mold::settings::MouseSensibility;
}

// tick update
void onTick()
{
    const float cameraSpeed = 20.0f * mold::time::DeltaTime; // adjust accordingly
    if (mold::input::GetKey(GLFW_KEY_UP))
        mold::render::camera::Rotate(mold::render::CameraDirection::Forward, cameraSpeed * 2);
    if (mold::input::GetKey(GLFW_KEY_DOWN))
        mold::render::camera::Rotate(mold::render::CameraDirection::Backwards, cameraSpeed * 2);
    if (mold::input::GetKey(GLFW_KEY_LEFT))
        mold::render::camera::Rotate(mold::render::CameraDirection::Left, cameraSpeed * 2);
    if (mold::input::GetKey(GLFW_KEY_RIGHT))
        mold::render::camera::Rotate(mold::render::CameraDirection::Right, cameraSpeed * 2);

    if (mold::input::GetKey('W'))
        mold::render::camera::Translate(mold::render::CameraDirection::Forward, cameraSpeed / 10);
    if (mold::input::GetKey('S'))
        mold::render::camera::Translate(mold::render::CameraDirection::Backwards, cameraSpeed / 10);
    if (mold::input::GetKey('A'))
        mold::render::camera::Translate(mold::render::CameraDirection::Left, cameraSpeed / 10);
    if (mold::input::GetKey('D'))
        mold::render::camera::Translate(mold::render::CameraDirection::Right, cameraSpeed / 10);

    if (mold::input::GetKey(GLFW_KEY_ESCAPE))
        mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Normal;

    if (mold::input::GetKey(GLFW_KEY_F1))
        mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Wrapped;
}

//draw on the screen
void onDraw()
{
}

//resize
void onResize()
{
    mold::log::Info("Resize!");
}

//exit
void onExit()
{
    mold::log::Info("Goodbye!");
}

//entry point
int main()
{
    if (!mold::Init(1024, 768))
        mold::Destroy();

    mold::input::GlobalCursorLockMode = mold::CursorLockingMode::Wrapped;

    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")), "Simple Cube");

    mold::GlobalGameObjects.Get("Simple Cube")->Move(glm::vec3(0, 1.0f, -1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Translate(glm::vec3(0, 1.0f, -1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Scale(glm::vec3(2.0f, 1.0f, 1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Opacity = 0.5f;

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw, onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick, onTick);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Resize, onResize);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Exit, onExit);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Mouse, onMouse);

    //Instantiate a cube
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255, 0, 0))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0, 255, 0))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0, 0, 255))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::GameObject());

    mold::GlobalGameObjects.Get("Textured Cube")->Move(glm::vec3(1.0f, 1.0f, 2.0f));
    mold::GlobalGameObjects.Get("Textured Cube 1")->Move(glm::vec3(-1.0f, 0.5f, -2.0f));
    mold::GlobalGameObjects.Get("Textured Cube 2")->Move(glm::vec3(1.0f, 0.0f, 2.0f));

    //main loop
    mold::Run();

    mold::Destroy();
    return 0;
}