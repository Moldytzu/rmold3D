#include <rmold3D/mold.h>

/*remaining features to add:
- basic logger
- internal console
- internal profiler
*/

void onTick()
{
    const float cameraSpeed = 60.0f * mold::time::DeltaTime; // adjust accordingly
    if (mold::input::GetKey('W'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Forward, cameraSpeed);
    if (mold::input::GetKey('S'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Backwards, cameraSpeed);
    if (mold::input::GetKey('A'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Left, cameraSpeed);
    if (mold::input::GetKey('D'))
        mold::render::camera::Rotate(mold::render::CameraDirection::Right, cameraSpeed);

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

    mold::GlobalGameObjects.Add("Simple Cube", new mold::render::objects::Cube(mold::render::image::LoadRGBBitmap("texture.bmp")));

    mold::GlobalGameObjects.Get("Simple Cube")->Move(glm::vec3(0, 1.0f, -1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Translate(glm::vec3(0, 1.0f, -1.0f));

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw, onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick, onTick);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Resize, onResize);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Exit, onExit);

    //Instantiate a cube
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::LoadRGBBitmap("texture.bmp")));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::LoadRGBBitmap("texture.bmp")));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::GameObject());
   
    //main loop
    mold::Run();

    mold::Destroy();
    return 0;
}