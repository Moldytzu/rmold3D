#include <rmold3D/mold.h>

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

unsigned int VBO;
unsigned int VAO;
unsigned int texture;

void onTick(GLFWwindow *window)
{
    const float cameraSpeed = 1.0f * deltaTime; // adjust accordingly
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        cameraPos += cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        cameraPos -= cameraSpeed * cameraFront;
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
}

//draw on the screen
void onDraw()
{
    glClearColor(0, 0, 0, 0);                           //black
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear screen

    // create transformations
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    projection = glm::perspective(glm::radians(mold::settings::FOV), mold::settings::WindowWidth / mold::settings::WindowHeight, 0.1f, 100.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // give the shader our view and projection
    glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    //draw basic cube
    glm::mat4 position = glm::mat4(1.0f);                                                           //model's position
    glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "model"), 1, GL_FALSE, &position[0][0]); // give the shader our position
    glBindTexture(GL_TEXTURE_2D, texture);                                                          //set the texture
    glBindVertexArray(VAO);                                                                         // vao of the cube with all the vertices
    glDrawArrays(GL_TRIANGLES, 0, 36);                                                              //draw 36 triangles
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
    if(!mold::Init(800,600))
        destroy();

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
    while (!glfwWindowShouldClose(mold::GlobalWindow))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //tick
        onTick(mold::GlobalWindow);

        //draw stuff
        onDraw();

        glfwSwapBuffers(mold::GlobalWindow);
        glfwPollEvents();
    }

    destroy();
    return 0;
}