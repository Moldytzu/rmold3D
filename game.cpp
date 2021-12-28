#include <rmold3D/mold.h>

/*remaining features to add
- basic logger
- internal console
- internal profiler
- gameobjects
*/

class GameObject
{
public:
    GameObject() {}
    virtual void Init() {}
    virtual void Draw() {}
    glm::mat4 Position = glm::mat4(1.0f);

protected:
    uint Texture = 0;
    mold::render::vabo::VABO Vabo;
    float* Vertices;
};

class Cube : public GameObject
{
public:
    Cube() {}

    void Init(mold::render::image::Texture texture)
    {
        Texture = mold::render::image::GenerateTextureIndex(texture);
        
        float vertices[] = {
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
            -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
            -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

        Vabo = mold::render::vabo::GenerateVABO(vertices, sizeof(vertices));
    }

    void Draw()
    {
        glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "model"), 1, GL_FALSE, &Position[0][0]); //give the shader our position
        glBindTexture(GL_TEXTURE_2D, Texture);                                                                                      //set the texture
        glBindBuffer(GL_ARRAY_BUFFER, Vabo.VBO);                                                                                    //vbo of the cube
        glBindVertexArray(Vabo.VAO);                                                                                                //vao of the cube
        glDrawArrays(GL_TRIANGLES, 0, 36);                                                                                          //draw 36 triangles
    }
};

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

Cube cube;

//draw on the screen
void onDraw()
{
    cube.Draw();
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

    cube.Init(mold::render::image::LoadRGBBitmap("texture.bmp"));

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw, onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick, onTick);

    //main loop
    mold::Run();

    destroy();
    return 0;
}