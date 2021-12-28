#include <rmold3D/mold.h>

#define WINDOW_HEIGHT 600
#define WINDOW_WIDTH 800

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
unsigned int shaderProgram;

//glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

//draw on the screen
void onDraw()
{
    glClearColor(0, 0, 0, 0);     //black
    glClear(GL_COLOR_BUFFER_BIT); //clear screen

    glUseProgram(shaderProgram);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
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

    //compile shaders
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 aPos;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n" //original position
                                     "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);\n" //white
                                       "}\n";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //create and compile vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //check for errors
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
        destroy();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create and compile fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
        destroy();

    shaderProgram = glCreateProgram(); //link the shaders together to form a program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
        destroy();

    //use shader
    glUseProgram(shaderProgram);

    //clean up the objects we don't need so we use less vram
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //triangles
    float vertices[] = {
        0.5f, 0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //set up a vbo, a vao & an ebo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                              //tell opengl we want to use this vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);       //set data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);                                      //tell opengl we want to use this ebo
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //set data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);   //explain how the data is arranged
    glEnableVertexAttribArray(0);                                                    //enable the first array

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        //draw stuff
        onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
    return 0;
}