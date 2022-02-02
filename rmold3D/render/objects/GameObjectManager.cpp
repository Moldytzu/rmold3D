#include <rmold3D/mold.h>

std::map<const char *, uint32_t> newObjects;

void mold::render::objects::GameObjectsManager::Add(const char *name, GameObject *object)
{
    GameObjects[name] = object;
}

void mold::render::objects::GameObjectsManager::Instantiate(GameObject *object)
{
    char *name = (char *)malloc(strlen(object->Type()) + 12);                                  // New Object xxxxxxxxxxxx    x -> digit
    memset(name, 0, strlen(object->Type()) + 12);                                              // clear
    strcpy(name, object->Type());                                                              // copy the first part
    strcpy(name + strlen(object->Type()), std::to_string(newObjects[object->Type()]).c_str()); // copy the number part
    mold::log::Info("Creating %s", name);                                                      //log debug
    Add(name, object);                                                                         // add it to our map
    free((void *)name);                                                                        // free up that heap space
    newObjects[object->Type()]++;                                                              //increment
}

bool mold::render::objects::GameObjectsManager::Exists(const char *name)
{
    return Get().contains(name);
}

void mold::render::objects::GameObjectsManager::Remove(const char *name)
{
    if (!Exists(name))
    {
        mold::log::Warn("Can't remove inexistent game object %s", name);
        return;
    }

    GameObjects.erase(name);
}

mold::render::objects::GameObject *mold::render::objects::GameObjectsManager::Get(const char *name)
{
    if (!Exists(name))
    {
        mold::log::Warn("Can't get inexistent game object %s", name);
        return nullptr;
    }

    return GameObjects[name];
}

std::map<const char *, mold::render::objects::GameObject *> mold::render::objects::GameObjectsManager::Get()
{
    return GameObjects;
}