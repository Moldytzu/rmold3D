#include <rmold3D/mold.h>

std::map<std::string, uint32_t> newObjects;

void mold::render::objects::GameObjectsManager::Add(std::string name, GameObject *object)
{
    GameObjects.emplace(std::move(name), std::move(object));
}

void mold::render::objects::GameObjectsManager::Instantiate(GameObject *object)
{
    Instantiate(object,object->Type());
}

void mold::render::objects::GameObjectsManager::Instantiate(GameObject *object, std::string name)
{
    std::string tname = name; // create a string based on the object's type

    if (newObjects[name] > 0)
        tname += " " + std::to_string(newObjects[name]); // append the object's no' if there are more then 1 object of that type

    Add(tname, object); // add our new object to our map/database with the other game objects
    newObjects[name]++; // increment the no' of instantiated objects with that type
}

bool mold::render::objects::GameObjectsManager::Exists(std::string name)
{
    return Get().contains(name);
}

void mold::render::objects::GameObjectsManager::Remove(std::string name)
{
    if (!Exists(name))
        {
            mold::log::Warn("Can't remove inexistent game object " + name);
            return;
        }

    GameObjects.erase(name);
}

mold::render::objects::GameObject *mold::render::objects::GameObjectsManager::Get(std::string name)
{
    if (!Exists(name))
        {
            mold::log::Warn("Can't get inexistent game object " + name);
            return nullptr;
        }

    return GameObjects[name];
}

std::map<std::string, mold::render::objects::GameObject *> mold::render::objects::GameObjectsManager::Get()
{
    return GameObjects;
}