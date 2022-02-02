#include <rmold3D/mold.h>

/*remaining features to add:
- mouse input
- lighting
- internal console
- internal profiler
*/

void onTick()
{
    const float cameraSpeed = 20.0f * mold::time::DeltaTime; // adjust accordingly
    if (mold::input::GetKey('W'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Forward, cameraSpeed * 2);
    if (mold::input::GetKey('S'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Backwards, cameraSpeed * 2);
    if (mold::input::GetKey('A'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Left, cameraSpeed * 2);
    if (mold::input::GetKey('D'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Right, cameraSpeed * 2);

    if (mold::input::GetKey(GLFW_KEY_UP))
        mold::render::camera::Translate(mold::render::CameraDirection::Forward, cameraSpeed / 10);
    if (mold::input::GetKey(GLFW_KEY_DOWN))
        mold::render::camera::Translate(mold::render::CameraDirection::Backwards, cameraSpeed / 10);
    if (mold::input::GetKey(GLFW_KEY_LEFT))
        mold::render::camera::Translate(mold::render::CameraDirection::Left, cameraSpeed / 10);
    if (mold::input::GetKey(GLFW_KEY_RIGHT))
        mold::render::camera::Translate(mold::render::CameraDirection::Right, cameraSpeed / 10);

    if (mold::input::GetKey(GLFW_KEY_ESCAPE))
        mold::input::LockCursor(mold::CursorLockingMode::Normal);

    if (mold::input::GetKey(GLFW_KEY_F1))
        mold::input::LockCursor(mold::CursorLockingMode::Locked);
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
    if (!mold::Init(800, 600))
        mold::Destroy();

    mold::input::LockCursor(mold::CursorLockingMode::Locked);

    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255,255,0))),"Simple Cube");

    mold::GlobalGameObjects.Get("Simple Cube")->Move(glm::vec3(0, 1.0f, -1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Translate(glm::vec3(0, 1.0f, -1.0f));

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw, onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick, onTick);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Resize, onResize);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Exit, onExit);

    //Instantiate a cube
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::GameObject());

    mold::GlobalGameObjects.Get("Textured Cube")->Move(glm::vec3(1.0f,1.0f,2.0f));
    mold::GlobalGameObjects.Get("Textured Cube 1")->Move(glm::vec3(-1.0f,0.5f,-2.0f));

    //main loop
    mold::Run();

    mold::Destroy();
    return 0;
}