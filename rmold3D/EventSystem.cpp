#include <rmold3D/mold.h>

using namespace mold;

void stub();

void EventSystem::AttachCallback(EventType type, void (*callback)())
{
    events[type] = callback;
}

void EventSystem::DetachCallback(EventType type)
{
    events[type] = stub;
}

std::map<EventType, void (*)()> EventSystem::GetMap()
{
    return events;
}