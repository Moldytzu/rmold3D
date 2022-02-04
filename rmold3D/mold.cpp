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

    //set rng
    srand((uint64_t)glfwGetTime());

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

    mold::log::Info("Rendering OpenGL " + std::string((const char *)glGetString(GL_VERSION)) + " on a " + std::string((const char *)glGetString(GL_VENDOR)) + " " + std::string((const char *)glGetString(GL_RENDERER)));

    // set up callbacks
    glfwSetFramebufferSizeCallback(mold::GlobalWindow, onResize);

    for (int enumInt = EventType::Redraw; enumInt != EventType::LAST; enumInt++) //iterate over each enum item
        GlobalEventSystem.AttachCallback(static_cast<EventType>(enumInt), stub);

    // set up gl
    glViewport(0, 0, width, height);
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //compile shaders
    unsigned int vertexShader = mold::render::shader::CompileShader(mold::render::shader::VertexShaderSource, GL_VERTEX_SHADER); //create and compile vertex shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(vertexShader))
    {
        int errorLen;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLen); //get len of error
        const char *buffer = new char[errorLen];                    //allocate memory for buffer
        int bufSize;
        glGetShaderInfoLog(vertexShader, errorLen, (GLsizei *)&bufSize, (GLchar *)buffer); //get info
        mold::log::Error("Vertex Shader: " + std::string(buffer));
        free((void *)buffer); //free up
        return false;
    }

    unsigned int fragmentShader = mold::render::shader::CompileShader(mold::render::shader::FragmentShaderSource, GL_FRAGMENT_SHADER); //create and compile fragment shader

    //check for errors
    if (!mold::render::shader::GetCompilationError(fragmentShader))
    {
        int errorLen;
        glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &errorLen); //get len of error
        const char *buffer = new char[errorLen];                    //allocate memory for buffer
        int bufSize;
        glGetShaderInfoLog(vertexShader, errorLen, (GLsizei *)&bufSize, (GLchar *)buffer); //get info
        mold::log::Error("Fragment Shader: " + std::string(buffer));
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

float oldYaw,oldPitch;
glm::vec3 oldDirection;

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

        //update window title
        std::string wtitle = "Rewritten mold3D @ ";
        wtitle += std::to_string((int)std::round((float)1/mold::time::DeltaTime));
        wtitle += " FPS";
        glfwSetWindowTitle(mold::GlobalWindow,wtitle.c_str());

        //update camera front
        if(mold::render::camera::Yaw != oldYaw || mold::render::camera::Pitch != oldPitch) // update only when the values change so we don't do cos and sin on every tick
        {
            oldDirection.x = cos(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
            oldDirection.y = sin(glm::radians(mold::render::camera::Pitch));
            oldDirection.z = sin(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
            mold::render::camera::Front = glm::normalize(oldDirection);
        
            oldYaw = mold::render::camera::Yaw; // save the values
            oldPitch = mold::render::camera::Pitch;
        }

        // create transformations
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        projection = glm::perspective(glm::radians(mold::settings::FOV), mold::settings::WindowWidth / mold::settings::WindowHeight, 0.1f, 100.0f);
        view = glm::lookAt(mold::render::camera::Position, mold::render::camera::Position + mold::render::camera::Front, mold::render::camera::Up);

        // give the shader our view and projection
        mold::render::shader::SetUniform4fv("view", view);
        mold::render::shader::SetUniform4fv("projection", projection);

        //use shader
        glUseProgram(mold::render::shader::GlobalShaderProgram);

        //draw stuff
        GlobalEventSystem.GetMap()[EventType::Redraw]();

        //draw game objects
        for (auto const &[name, ptr] : GlobalGameObjects.Get())
        {
            if (ptr->Enabled) //don't render empty gameobjects
            {
                ptr->Bind(); // bind vabo, texture and matrices
                ptr->Draw(); // do drawing
            }
        }

        glFlush();

        glfwSwapBuffers(mold::GlobalWindow);
        glfwPollEvents();
    }
}