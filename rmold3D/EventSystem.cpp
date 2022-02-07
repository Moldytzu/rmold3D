#include <rmold3D/mold.h>

using namespace mold;

void EventSystem::AttachCallback(EventType type, void (*callback)())
{
    events.emplace(type, callback); // build std::pair and insert it in the map
}

void EventSystem::DetachCallback(EventType type)
{
    events.erase(type);
}

bool EventSystem::ExistsCallback(EventType type)
{
    return events.contains(type);
}

std::unordered_map<EventType, void (*)()> EventSystem::GetMap()
{
    return events; // return map
}

void EventSystem::CallEvent(EventType type)
{
    for (auto const &[name, ptr] : GlobalGameObjects.Get())
    {
        if (ptr->Enabled) // don't handle events on disabled gameobjects
        {
            ptr->HandleComponents(type); // handle events
        }
    }

    if (!ExistsCallback(type))
        return; // return if the callback doesn't exist

    events[type](); // call event
}