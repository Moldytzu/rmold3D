#include "mold.h"

using namespace mold::input;

bool mold::input::GetKey(int key)
{
    return glfwGetKey(GlobalWindow, key); // wrap glfwGetKey
}