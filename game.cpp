#include <rmold3D/mold.h>

#define FOV 90.0f

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

float deltaTime = 0.0f;
float lastFrame = 0.0f;

#define WINDOW_HEIGHT (float)600
#define WINDOW_WIDTH (float)800

#define SetUniformf(name, value) glUniform1f(glGetUniformLocation(shaderProgram, name), value);
#define SetUniformi(name, value) glUniform1i(glGetUniformLocation(shaderProgram, name), value);

unsigned int VBO;
unsigned int VAO;
unsigned int shaderProgram;
unsigned int texture;

//glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

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
    projection = glm::perspective(glm::radians(FOV), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    // give the shader our view and projection
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    //draw basic cube
    glm::mat4 position = glm::mat4(1.0f);                                                           //model's position
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &position[0][0]); // give the shader our position
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
    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rewritten mold 3D", NULL, NULL); // create window
    if (window == NULL)                                                                                  // exit if the window couldn't be created
        destroy();

    glfwMakeContextCurrent(window); // create opengl context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
        destroy();

    // set up callbacks
    glfwSetFramebufferSizeCallback(window, onResize);

    // set up gl
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.1f, 100.0f);
    glEnable(GL_DEPTH_TEST);

    //compile shaders
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 vertexPosition;\n"
                                     "layout (location = 1) in vec2 textureCoordornate;\n"
                                     "out vec2 textureCoord;\n"
                                     "uniform mat4 model;\n"
                                     "uniform mat4 view;\n"
                                     "uniform mat4 projection;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
                                     "   textureCoord = textureCoordornate;\n"
                                     "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "in vec2 textureCoord;\n"
                                       "uniform sampler2D mainTexture;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = texture(mainTexture, textureCoord);\n"
                                       "}\n";

    unsigned int vertexShader = mold::render::shader::CompileShader(vertexShaderSource, GL_VERTEX_SHADER); //create and compile vertex shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(vertexShader))
        destroy();

    unsigned int fragmentShader = mold::render::shader::CompileShader(fragmentShaderSource, GL_FRAGMENT_SHADER); //create and compile fragment shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(fragmentShader))
        destroy();

    shaderProgram = mold::render::shader::LinkShader(fragmentShader, vertexShader); //link the shaders together to form a program

    //check for errors
    if (!mold::render::shader::GetLinkError(shaderProgram))
        destroy();

    //use shader
    glUseProgram(shaderProgram);

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

    //set up a vbo, a vao & an ebo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    //glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        //tell opengl we want to use this vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //set data
    //glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    //glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    // texture coord attribute
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //load texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    mold::render::image::Texture t = mold::render::image::LoadRGBBitmap("texture.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.Width, t.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, t.PixelData);
    glGenerateMipmap(GL_TEXTURE_2D);

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        //tick
        onTick(window);

        //draw stuff
        onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
    return 0;
}