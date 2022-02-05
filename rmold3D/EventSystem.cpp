#include <rmold3D/mold.h>

using namespace mold;

void stub();

void EventSystem::AttachCallback(EventType type, void (*callback)())
{
    events[type] = callback; // set event
}

void EventSystem::DetachCallback(EventType type)
{
    events[type] = stub; // set to the default stub
}

std::map<EventType, void (*)()> EventSystem::GetMap()
{
    return events; // return map
}