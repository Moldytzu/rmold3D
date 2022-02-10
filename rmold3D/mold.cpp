#include <rmold3D/mold.h>

void handleScroll(GLFWwindow *, double, double);

// glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    mold::GlobalEventSystem.CallEvent(mold::EventType::Resize); // call resize event
    mold::settings::WindowHeight = height;                      // set new window height and width
    mold::settings::WindowWidth = height;
    glViewport(0, 0, width, height);                                   // set new viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // reset orthography
}

// clean up
void mold::Destroy()
{
    mold::GlobalEventSystem.CallEvent(mold::EventType::Exit); // call exit event

    for (auto const &[name, ptr] : GlobalGameObjects.Get())
        delete ptr; // delete gameobject

    mold::render::objects::GameObject tmp;
    for (auto const &[name, ptr] : tmp.GetComponents())
        delete ptr; // delete all components

    delete mold::GlobalApplication; // deconstuct application

    glfwTerminate(); // terminate glfw

    exit(0); // exit
}

void mold::Init(uint width, uint height)
{
    mold::settings::WindowHeight = height;
    mold::settings::WindowWidth = width;

    mold::log::Info("Starting Rewritten mold 3D");

    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core

    // set rng
    srand((uint64_t)glfwGetTime());

    mold::GlobalWindow = glfwCreateWindow(width, height, "Rewritten mold 3D", NULL, NULL); // create window
    if (mold::GlobalWindow == NULL)                                                        // exit if the window couldn't be created
        mold::log::Fatal("Couldn't create glfw window!");

#ifdef GLFW_RAW_MOUSE_MOTION
    if (glfwRawMouseMotionSupported()) // enable raw mouse motion if supported
        glfwSetInputMode(mold::GlobalWindow, GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
#endif

    glfwMakeContextCurrent(mold::GlobalWindow); // create opengl context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
        mold::log::Fatal("Couldn't load glad!");

    mold::log::Info("Rendering OpenGL " + std::string((const char *)glGetString(GL_VERSION)) + " on a " + std::string((const char *)glGetString(GL_VENDOR)) + " " + std::string((const char *)glGetString(GL_RENDERER))); // display opengl information

    // set up glfw callbacks
    glfwSetFramebufferSizeCallback(mold::GlobalWindow, onResize);
    glfwSetScrollCallback(mold::GlobalWindow, handleScroll);

    // set up gl
    glViewport(0, 0, width, height);                                   // set viewport
    glm::ortho(0.0f, (float)width, 0.0f, (float)height, 0.1f, 100.0f); // set orthographic projection
    glEnable(GL_DEPTH_TEST);                                           // depth testing
    glEnable(GL_BLEND);                                                // blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);                 // substract alpha channel for transparency

    GlobalShader.AttachSource(mold::render::VertexShaderSource, GL_VERTEX_SHADER);     // attach vertex shader
    GlobalShader.AttachSource(mold::render::FragmentShaderSource, GL_FRAGMENT_SHADER); // attach fragment shader
    GlobalShader.Recompile();                                                          // compile it
    GlobalShader.Bind();
}

// mouse handling
double lastX;
double lastY;

void handleScroll(GLFWwindow *window, double xoffset, double yoffset)
{
    mold::input::GlobalScrollAxis = yoffset; // the scroll wheel in glfw works as a mouse where the yoffset is the cursor axis itself
}

void handleMouse()
{
    // handle mouse input
    double xpos, ypos;
    glfwGetCursorPos(mold::GlobalWindow, &xpos, &ypos); // get cursor position

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

    // set axis data
    mold::input::GlobalCursorAxis.x = xpos - lastX;
    mold::input::GlobalCursorAxis.y = lastY - ypos;

    mold::input::GlobalCursorPos.x = xpos;
    mold::input::GlobalCursorPos.y = ypos;

    // call event if we've got changes
    if ((mold::input::GlobalCursorAxis.x != 0 && mold::input::GlobalCursorAxis.y != 0) || mold::input::GlobalScrollAxis != 0)
        mold::GlobalEventSystem.CallEvent(mold::EventType::Mouse);

    // update last values
    lastX = xpos;
    lastY = ypos;
    mold::input::GlobalScrollAxis = 0; // reset scroll value
}

float oldYaw, oldPitch;
glm::vec3 oldDirection;

void mold::Run()
{
    mold::log::Info("Running " + mold::GlobalApplication->Name());

    // run the game if the window is open
    while (!glfwWindowShouldClose(mold::GlobalWindow))
    {
        // calculate delta time
        float currentFrame = glfwGetTime();
        mold::time::DeltaTime = currentFrame - mold::time::LastFrame;
        mold::time::LastFrame = currentFrame;

        // call tick
        mold::GlobalEventSystem.CallEvent(EventType::Tick);

        glClearColor(0, 0, 0, 0);                           // black
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear screen and the depth buffer

        // update window title (Rewritten mold 3D @ ?? FPS)
        std::string wtitle = "Rewritten mold 3D @ ";
        wtitle += std::to_string((int)std::round((float)1 / mold::time::DeltaTime));
        wtitle += " FPS";
        glfwSetWindowTitle(mold::GlobalWindow, wtitle.c_str());

        // handle cursor locking mode
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

        // do mouse handling
        handleMouse();

        // update camera front
        if (mold::render::camera::Yaw != oldYaw || mold::render::camera::Pitch != oldPitch) // update only when the values change so we don't do cos and sin on every tick
        {
            // clamp the value of pitch
            mold::render::camera::Pitch = glm::clamp(mold::render::camera::Pitch, -89.0f, 89.0f);

            oldDirection.x = cos(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
            oldDirection.y = sin(glm::radians(mold::render::camera::Pitch));
            oldDirection.z = sin(glm::radians(mold::render::camera::Yaw)) * cos(glm::radians(mold::render::camera::Pitch));
            mold::render::camera::Front = glm::normalize(oldDirection);

            oldYaw = mold::render::camera::Yaw; // save the values
            oldPitch = mold::render::camera::Pitch;
        }

        // ensure that we use shader
        GlobalShader.Bind();

        // give the shader our view and projection
#define view glm::lookAt(mold::render::camera::Position, mold::render::camera::Position + mold::render::camera::Front, mold::render::camera::Up)
#define projection glm::perspective(glm::radians(mold::settings::FOV), mold::settings::WindowWidth / mold::settings::WindowHeight, 0.1f, 100.0f)

        GlobalShader.Set("view", view);
        GlobalShader.Set("projection", projection);
        GlobalShader.Set("sunPos", mold::render::lighting::SunPosition);

        // draw stuff
        mold::GlobalEventSystem.CallEvent(EventType::Redraw);

        // draw game objects
        for (auto const &[name, ptr] : GlobalGameObjects.Get())
        {
            if (!ptr->Enabled)
                continue;

            if (mold::render::camera::InView(ptr->GetPosition()) && ptr->Type() != "Empty") // draw if the object is in view and if it isn't an empty gameobject
            {
                ptr->Bind(); // bind vabo, texture and matrices
                ptr->Draw(); // do drawing
            }
            ptr->TickComponents(); // tick it's components
        }

        glFlush(); // flush pipeline

        glfwSwapBuffers(mold::GlobalWindow); // swap buffers
        glfwPollEvents();                    // poll events
    }
}