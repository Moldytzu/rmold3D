#include <rmold3D/mold.h>

mold::render::objects::Empty::Empty() : mold::render::objects::GameObject(mold::render::image::Texture(mold::render::Colour(0))) {} // wrap the ugly constructor of gameobject