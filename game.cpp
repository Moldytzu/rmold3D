#include <rmold3D/mold.h>

/*remaining features to add:
- mouse input
- lighting
- internal console
- internal profiler
*/

//mouse movement
double lastX;
double lastY;

double mouseSens = 0.005f;

void onTick()
{
// handle mouse input for camera
double xpos, ypos;
glfwGetCursorPos(mold::GlobalWindow,&xpos, &ypos);

if(xpos > mold::settings::WindowWidth)
    glfwSetCursorPos(mold::GlobalWindow,0,ypos);

if(ypos > mold::settings::WindowHeight)
    glfwSetCursorPos(mold::GlobalWindow,xpos,0);

    if(lastX != xpos)
    {
        double offsetX = xpos-lastX;
        
        mold::render::camera::Yaw += offsetX * mouseSens;

        lastX = xpos;
    }

    if(lastY != ypos)
    {
        double offsetY = lastY-ypos;

        mold::render::camera::Pitch += offsetY * mouseSens ;

        lastY = ypos;
    }

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
        mold::input::LockCursor(mold::CursorLockingMode::Centred);
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



bool first = true;
void onMouse(GLFWwindow* window, double xpos, double ypos)
{
    if(first)
    {
        lastX = xpos;
        lastY = ypos;
        first = false;
        return;
    }

}

//entry point
int main()
{
    if (!mold::Init(800, 600))
        mold::Destroy();

if (glfwRawMouseMotionSupported())
    glfwSetInputMode(mold::GlobalWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

    glfwSetCursorPos(mold::GlobalWindow,400,300);

    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture("texture.bmp")),"Simple Cube");

    mold::GlobalGameObjects.Get("Simple Cube")->Move(glm::vec3(0, 1.0f, -1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Translate(glm::vec3(0, 1.0f, -1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Scale(glm::vec3(2.0f,1.0f,1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Opacity = 0.5f;

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw, onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick, onTick);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Resize, onResize);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Exit, onExit);

    //Instantiate a cube
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255,0,0))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0,255,0))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(0,0,255))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::Cube(mold::render::image::Texture(mold::render::Colour(255))));
    mold::GlobalGameObjects.Instantiate(new mold::render::objects::GameObject());

    mold::GlobalGameObjects.Get("Textured Cube")->Move(glm::vec3(1.0f,1.0f,2.0f));
    mold::GlobalGameObjects.Get("Textured Cube 1")->Move(glm::vec3(-1.0f,0.5f,-2.0f));
    mold::GlobalGameObjects.Get("Textured Cube 2")->Move(glm::vec3(1.0f,0.0f,2.0f));

    //main loop
    mold::Run();

    mold::Destroy();
    return 0;
}