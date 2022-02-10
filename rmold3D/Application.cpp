#include "mold.h"

using namespace mold;

void mold::Application::OnDraw() {}
void mold::Application::OnMouseInput() {}
void mold::Application::OnResize() {}
void mold::Application::Tick() {}

std::string mold::Application::Name()
{
    return "Game";
}

mold::Application::Application() : mold::Application::Application(800, 600) {}

mold::Application::Application(uint width, uint height)
{
    Init(width, height);
}

int main()
{
    GlobalApplication = BuildApplication(); // ask the sandbox to build an application
    Run();                                  // run the engine
    Destroy();                              // clean everything
    return 0;
}