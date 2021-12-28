#include <rmold3D/mold.h>

using namespace mold;

bool mold::input::GetKey(int key)
{
    return glfwGetKey(mold::GlobalWindow, key);
}

void mold::input::LockCursor(CursorLockingMode locked)
{
    if (locked == CursorLockingMode::Normal)
        glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    else if (locked == CursorLockingMode::Locked)
        glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    else
        glfwSetInputMode(mold::GlobalWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
}