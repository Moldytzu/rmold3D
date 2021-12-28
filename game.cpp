#include <rmold3D/mold.h>

unsigned int VBO;
unsigned int VAO;
unsigned int texture;

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
    //draw basic cube
    glm::mat4 position = glm::mat4(1.0f);                                                                                       //model's position
    glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "model"), 1, GL_FALSE, &position[0][0]); // give the shader our position
    glBindTexture(GL_TEXTURE_2D, texture);                                                                                      //set the texture
    glBindVertexArray(VAO);                                                                                                     // vao of the cube with all the vertices
    glDrawArrays(GL_TRIANGLES, 0, 36);                                                                                          //draw 36 triangles
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

    //Callbacks
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Redraw,onDraw);
    mold::GlobalEventSystem.AttachCallback(mold::EventType::Tick,onTick);

    //Cube
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

    //set up a vbo & a vao
    mold::render::vabo::VABO vabo = mold::render::vabo::GenerateVABO(vertices, sizeof(vertices));
    VAO = vabo.VAO;
    VBO = vabo.VBO;

    //load texture
    texture = mold::render::image::GenerateTextureIndex(mold::render::image::LoadRGBBitmap("texture.bmp"));

    //main loop
    mold::Run();

    destroy();
    return 0;
}