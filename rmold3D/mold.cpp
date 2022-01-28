#include <rmold3D/mold.h>

void stub() {}

//glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    mold::GlobalEventSystem.GetMap()[mold::EventType::Resize]();
    mold::settings::WindowHeight = height;
    mold::settings::WindowWidth = height;
    glViewport(0, 0, width, height);
}

//clean up
void mold::Destroy()
{
    GlobalEventSystem.GetMap()[EventType::Exit]();

    glfwTerminate(); //terminate glfw

    for (std::pair<const char *, render::objects::GameObject *> object : GlobalGameObjects.Get())
        free(object.second); //free objects

    exit(0); //exit
}

bool mold::Init(uint width, uint height)
{
    mold::settings::WindowHeight = height;
    mold::settings::WindowWidth = width;

    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core

    mold::GlobalWindow = glfwCreateWindow(width, height, "Rewritten mold 3D", NULL, NULL); // create window
    if (mold::GlobalWindow == NULL)                                                        // exit if the window couldn't be created
    {
        mold::log::Error("Couldn't create glfw window!");
        return false;
    }

    glfwMakeContextCurrent(mold::GlobalWindow); // create opengl context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
    {
        mold::log::Error("Couldn't load glad!");
        return false;
    }

    // set up callbacks
    glfwSetFramebufferSizeCallback(mold::GlobalWindow, onResize);

    for (int enumInt = EventType::Redraw; enumInt != EventType::LAST; enumInt++) //iterate over each enum item
        GlobalEventSystem.AttachCallback(static_cast<EventType>(enumInt), stub);

    // set up gl
    glViewport(0, 0, width, height);
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);
    glEnable(GL_DEPTH_TEST);

    //compile shaders
    unsigned int vertexShader = mold::render::shader::CompileShader(mold::render::shader::VertexShaderSource, GL_VERTEX_SHADER); //create and compile vertex shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(vertexShader))
    {
        int errorLen;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLen); //get len of error
        const char *buffer = (const char *)malloc(errorLen);        //allocate memory for buffer
        int bufSize;
        glGetShaderInfoLog(vertexShader, errorLen, (GLsizei *)&bufSize, (GLchar *)buffer); //get info
        mold::log::Error("Vertex Shader: %s", buffer);
        free((void *)buffer); //free up
        return false;
    }

    unsigned int fragmentShader = mold::render::shader::CompileShader(mold::render::shader::FragmentShaderSource, GL_FRAGMENT_SHADER); //create and compile fragment shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(fragmentShader))
    {
        int errorLen;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLen); //get len of error
        const char *buffer = (const char *)malloc(errorLen);        //allocate memory for buffer
        int bufSize;
        glGetShaderInfoLog(vertexShader, errorLen, (GLsizei *)&bufSize, (GLchar *)buffer); //get info
        mold::log::Error("Fragment Shader: %s", buffer);
        free((void *)buffer); //free up
        return false;
    }

    mold::render::shader::GlobalShaderProgram = mold::render::shader::LinkShader(fragmentShader, vertexShader); //link the shaders together to form a program

    //check for errors
    if (!mold::render::shader::GetLinkError(mold::render::shader::GlobalShaderProgram))
    {
        mold::log::Error("Failed to link shader program");
        return false;
    }

    //use shader
    glUseProgram(mold::render::shader::GlobalShaderProgram);

    return true;
}

void mold::Run()
{
    while (!glfwWindowShouldClose(mold::GlobalWindow))
    {
        float currentFrame = glfwGetTime();
        mold::time::DeltaTime = currentFrame - mold::time::LastFrame;
        mold::time::LastFrame = currentFrame;

        //tick
        GlobalEventSystem.GetMap()[EventType::Tick]();

        glClearColor(0, 0, 0, 0);                           //black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear screen

        //update camera front
        glm::vec3 direction;
        direction.x = cos(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
        direction.y = sin(glm::radians(mold::render::camera::Pitch));
        direction.z = sin(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
        mold::render::camera::Front = glm::normalize(direction);

        // create transformations
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(mold::settings::FOV), mold::settings::WindowWidth / mold::settings::WindowHeight, 0.1f, 100.0f);
        view = glm::lookAt(mold::render::camera::Position, mold::render::camera::Position + mold::render::camera::Front, mold::render::camera::Up);

        // give the shader our view and projection
        glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
        glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

        //use shader
        glUseProgram(mold::render::shader::GlobalShaderProgram);

        //draw stuff
        GlobalEventSystem.GetMap()[EventType::Redraw]();

        //draw game objects
        for (std::pair<const char *, render::objects::GameObject *> object : GlobalGameObjects.Get())
        {
            if (object.second->Enabled)
                object.second->Draw();
        }

        glFlush();

        glfwSwapBuffers(mold::GlobalWindow);
        glfwPollEvents();
    }
}