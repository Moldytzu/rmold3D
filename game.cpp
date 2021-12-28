#include <rmold3D/mold.h>

/*remaining features to add:
- basic logger
- internal console
- internal profiler
*/

void onTick()
{
    const float cameraSpeed = 1.0f * mold::time::DeltaTime; // adjust accordingly
    if (glfwGetKey(mold::GlobalWindow, GLFW_KEY_W) == GLFW_PRESS)
        mold::render::camera::Position += cameraSpeed * mold::render::camera::Front;
    if (glfwGetKey(mold::GlobalWindow, GLFW_KEY_S) == GLFW_PRESS)
        mold::render::camera::Position -= cameraSpeed * mold::render::camera::Front;
    if (glfwGetKey(mold::GlobalWindow, GLFW_KEY_A) == GLFW_PRESS)
        mold::render::camera::Position -= glm::normalize(glm::cross(mold::render::camera::Front, mold::render::camera::Up)) * cameraSpeed;
    if (glfwGetKey(mold::GlobalWindow, GLFW_KEY_D) == GLFW_PRESS)
        mold::render::camera::Position += glm::normalize(glm::cross(mold::render::camera::Front, mold::render::camera::Up)) * cameraSpeed;
}

//draw on the screen
void onDraw()
{
    
}

//clean up the mess
void destroy()
{
    glfwTerminate();
    exit(-1);
}

//entry point
int main()
{
    if (!mold::Init(800, 600))
        destroy();

    mold::GlobalGameObjects.Add("Simple Cube",new mold::render::objects::Cube(mold::render::image::LoadRGBBitmap("texture.bmp")));

    mold::GlobalGameObjects.Get("Simple Cube")->Move(glm::vec3(0,1.0f,-1.0f));
    mold::GlobalGameObjects.Get("Simple Cube")->Translate(glm::vec3(0,1.0f,-1.0f));

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw, onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick, onTick);

    //main loop
    mold::Run();

    destroy();
    return 0;
}