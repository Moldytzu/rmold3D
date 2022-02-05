#include <rmold3D/mold.h>

using namespace mold;

bool mold::input::GetKey(int key)
{
    return glfwGetKey(mold::GlobalWindow, key); // wrap glfwGetKey
}