#include "mold.h"

using namespace mold;

void EventSystem::CallEvent(EventType type)
{
    for (auto const &[name, ptr] : GlobalGameObjects.Get())
    {
        if (ptr->Enabled) // don't handle events on disabled gameobjects
        {
            ptr->HandleComponents(type); // handle events
        }
    }

    switch (type) // call the event
    {
    case EventType::Tick:
        GlobalApplication->Tick();
        break;
    case EventType::Redraw:
        GlobalApplication->OnDraw();
        break;
    case EventType::Mouse:
        GlobalApplication->OnMouseInput();
        break;
    case EventType::Resize:
        GlobalApplication->OnResize();
        break;

    default:
        break;
    }
}