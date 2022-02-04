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

    mold::log::Info("Starting Rewritten mold 3D");

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

    if (glfwRawMouseMotionSupported()) // enable raw mouse motion if supported
        glfwSetInputMode(mold::GlobalWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);

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
    uint vertexShader = mold::render::shader::CompileShader(mold::render::shader::VertexShaderSource, GL_VERTEX_SHADER); //create and compile vertex shader

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

    uint fragmentShader = mold::render::shader::CompileShader(mold::render::shader::FragmentShaderSource, GL_FRAGMENT_SHADER); //create and compile fragment shader

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

// mouse handling
double lastX;
double lastY;

void handleMouse()
{
    // handle mouse input for camera
    double xpos, ypos;
    glfwGetCursorPos(mold::GlobalWindow, &xpos, &ypos);

    // wrap cursor around window borders
    if (mold::input::GlobalCursorLockMode == mold::CursorLockingMode::Wrapped)
    {
        if (xpos > mold::settings::WindowWidth)
        {
            glfwSetCursorPos(mold::GlobalWindow, 10, ypos);
            lastX = 10;
            return;
        }

        if (xpos < 0)
        {
            glfwSetCursorPos(mold::GlobalWindow, mold::settings::WindowWidth - 10, ypos);
            lastX = mold::settings::WindowWidth - 10;
            return;
        }

        if (ypos > mold::settings::WindowHeight)
        {
            glfwSetCursorPos(mold::GlobalWindow, xpos, 10);
            lastY = 10;
            return;
        }

        if (ypos < 0)
        {
            glfwSetCursorPos(mold::GlobalWindow, xpos, mold::settings::WindowHeight - 10);
            lastY = mold::settings::WindowHeight - 10;
            return;
        }
    }

    mold::input::GlobalCursorAxisX = xpos - lastX;
    mold::input::GlobalCursorAxisY = lastY - ypos;

    mold::GlobalEventSystem.GetMap()[mold::EventType::Mouse]();

    // update last values
    lastX = xpos;
    lastY = ypos;
}

float oldYaw, oldPitch;
glm::vec3 oldDirection;

void mold::Run()
{
    mold::log::Info("Running the game");
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
        std::string wtitle = "Rewritten mold 3D @ ";
        wtitle += std::to_string((int)std::round((float)1 / mold::time::DeltaTime));
        wtitle += " FPS";
        glfwSetWindowTitle(mold::GlobalWindow, wtitle.c_str());

        //handle cursor locking mode
        switch (input::GlobalCursorLockMode)
        {
        case CursorLockingMode::Normal:
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CursorLockingMode::Locked:
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case CursorLockingMode::Centred:
            glfwSetCursorPos(mold::GlobalWindow, mold::settings::WindowWidth / 2, mold::settings::WindowHeight / 2);
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }

        //do mouse handling
        handleMouse();

        //update camera front
        if (mold::render::camera::Yaw != oldYaw || mold::render::camera::Pitch != oldPitch) // update only when the values change so we don't do cos and sin on every tick
        {
            //clamp the value of pitch
            mold::render::camera::Pitch = std::clamp(mold::render::camera::Pitch, -89.0f, 89.0f);

            oldDirection.x = cos(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
            oldDirection.y = sin(glm::radians(mold::render::camera::Pitch));
            oldDirection.z = sin(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
            mold::render::camera::Front = glm::normalize(oldDirection);

            oldYaw = mold::render::camera::Yaw; // save the values
            oldPitch = mold::render::camera::Pitch;
        }

        // give the shader our view and projection

#define view glm::lookAt(mold::render::camera::Position, mold::render::camera::Position + mold::render::camera::Front, mold::render::camera::Up)
#define projection glm::perspective(glm::radians(mold::settings::FOV), mold::settings::WindowWidth / mold::settings::WindowHeight, 0.1f, 100.0f)

        mold::render::shader::SetUniform4fv("view", view);
        mold::render::shader::SetUniform4fv("projection", projection);

        //use shader
        glUseProgram(mold::render::shader::GlobalShaderProgram);

        //draw stuff
        GlobalEventSystem.GetMap()[EventType::Redraw]();

        //draw game objects
        for (auto const &[name, ptr] : GlobalGameObjects.Get())
        {
            if (ptr->Enabled) //don't render disabled gameobjects
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