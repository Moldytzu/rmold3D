#include <rmold3D/mold.h>

void mold::Application::OnDraw() {}
void mold::Application::OnMouseInput() {}
void mold::Application::OnResize() {}
void mold::Application::Tick() {}

mold::Application::Application() : mold::Application::Application(800, 600) {}

mold::Application::Application(uint width, uint height)
{
    mold::Init(width, height);
}

int main()
{
    mold::GlobalApplication = mold::BuildApplication(); // ask the sandbox to build an application
    mold::Run();                                        // run the engine
    mold::Destroy();                                    // clean everything
    return 0;
}