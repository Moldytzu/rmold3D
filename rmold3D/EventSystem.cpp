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

std::map<EventType, void (*)()> EventSystem::GetMap()
{
    return events; // return map
}

void EventSystem::CallEvent(EventType type)
{
    if(!ExistsCallback(type)) return; // return if the callback doesn't exist

    events[type](); // call event
}