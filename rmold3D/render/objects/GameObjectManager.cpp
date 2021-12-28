#include <rmold3D/mold.h>

void mold::render::objects::GameObjectsManager::Add(const char *name, GameObject *object)
{
    GameObjects[name] = object;
}

void mold::render::objects::GameObjectsManager::Remove(const char *name)
{
    GameObjects.erase(name);
}

mold::render::objects::GameObject *mold::render::objects::GameObjectsManager::Get(const char *name)
{
    return GameObjects[name];
}

std::map<const char *, mold::render::objects::GameObject *> mold::render::objects::GameObjectsManager::Get()
{
    return GameObjects;
}