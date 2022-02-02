#include <rmold3D/mold.h>

std::map<const char *, uint32_t> newObjects;

void mold::render::objects::GameObjectsManager::Add(const char *name, GameObject *object)
{
    GameObjects[name] = object;
}

void mold::render::objects::GameObjectsManager::Instantiate(GameObject *object)
{
    mold::log::Info("Creating %s with the number %u", object->Type(), newObjects[object->Type()]); //log debug
    char *name = (char *)malloc(strlen("New Object ") + 12);                                       // New Object xxxxxxxxxxxx    x -> digit
    strcpy(name, "New Object ");                                                                   // copy the first part
    strcpy(name + strlen("New Object "), std::to_string(newObjects[object->Type()]).c_str());      // copy the number part
    Add(name, object);                                                                             // add it to our map
    free((void *)name);                                                                            // free up that heap space
    newObjects[object->Type()]++;                                                                  //increment
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