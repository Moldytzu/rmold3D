#include "../../mold.h"

using namespace mold::render::objects;

mold::render::objects::Empty::Empty() : GameObject(image::Texture(Colour(0))) {} // wrap the ugly constructor of gameobject