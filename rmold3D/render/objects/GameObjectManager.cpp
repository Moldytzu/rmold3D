#include <rmold3D/mold.h>

uint32_t newObjects = 0;

void mold::render::objects::GameObjectsManager::Add(const char *name, GameObject *object)
{
    GameObjects[name] = object;
}

void mold::render::objects::GameObjectsManager::Instantiate(GameObject *object)
{
    char *name = (char*)malloc(strlen("New Object ") + 12); // New Object xxxxxxxxxxxx    x -> digit
    strcpy(name,"New Object "); // copy the first part
    strcpy(name + strlen("New Object "),std::to_string(newObjects).c_str()); // copy the number part
    Add(name,object); //add it to our map
    free((void*)name); //free up that heap space
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