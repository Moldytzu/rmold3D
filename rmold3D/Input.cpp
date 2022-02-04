#include <rmold3D/mold.h>

using namespace mold;

bool mold::input::GetKey(int key)
{
    return glfwGetKey(mold::GlobalWindow, key);
}

void mold::input::LockCursor(CursorLockingMode locked)
{
    switch(locked)
        {
        case CursorLockingMode::Normal:
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
            break;
        case CursorLockingMode::Locked:
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        case CursorLockingMode::Centred:
            glfwSetCursorPos(mold::GlobalWindow,mold::settings::WindowWidth/2,mold::settings::WindowHeight/2);
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
            break;
        default:
            glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
            break;
        }
}