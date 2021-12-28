#include <rmold3D/mold.h>

//glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

bool mold::Init(uint width, uint height)
{
    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core

    mold::GlobalWindow = glfwCreateWindow(width, height, "Rewritten mold 3D", NULL, NULL); // create window
    if (mold::GlobalWindow == NULL)                                                        // exit if the window couldn't be created
        return false;

    glfwMakeContextCurrent(mold::GlobalWindow); // create opengl context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
        return false;

    // set up callbacks
    glfwSetFramebufferSizeCallback(mold::GlobalWindow, onResize);

    // set up gl
    glViewport(0, 0, width, height);
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);
    glEnable(GL_DEPTH_TEST);

    //compile shaders
    unsigned int vertexShader = mold::render::shader::CompileShader(mold::render::shader::VertexShaderSource, GL_VERTEX_SHADER); //create and compile vertex shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(vertexShader))
        return false;

    unsigned int fragmentShader = mold::render::shader::CompileShader(mold::render::shader::FragmentShaderSource, GL_FRAGMENT_SHADER); //create and compile fragment shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(fragmentShader))
        return false;

    mold::render::shader::GlobalShaderProgram = mold::render::shader::LinkShader(fragmentShader, vertexShader); //link the shaders together to form a program

    //check for errors
    if (!mold::render::shader::GetLinkError(mold::render::shader::GlobalShaderProgram))
        return false;

    //use shader
    glUseProgram(mold::render::shader::GlobalShaderProgram);

    return true;
}