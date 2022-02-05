#include <rmold3D/mold.h>

mold::render::objects::GameObject::GameObject() {}

void mold::render::objects::GameObject::Draw() {}

mold::render::objects::GameObject::~GameObject()
{
    Texture.Deallocate(); // deallocate texture and vabo
    Vabo.Deallocate();
}

mold::render::objects::GameObject::GameObject(mold::render::image::Texture texture)
{
    Texture = texture; // set texture and enable the object
    Enabled = true;
}

void mold::render::objects::GameObject::Translate(glm::vec3 offset)
{
    PositionMatrix = glm::translate(PositionMatrix, offset); // translate matrix
}

void mold::render::objects::GameObject::Move(glm::vec3 position)
{
    PositionMatrix = glm::translate(glm::mat4(1.0f), position); // move matrix by translating a new one to the desired position
}

void mold::render::objects::GameObject::Scale(glm::vec3 scaleFactor)
{
    PositionMatrix = glm::scale(PositionMatrix, scaleFactor); // scale matrix
}

void mold::render::objects::GameObject::ReplaceTexture(mold::render::image::Texture newTexture)
{
    Texture.Deallocate(); // deallocate so we don't create memory leaks or smth
    Texture = newTexture; // replace the old texture
}

void mold::render::objects::GameObject::Bind()
{
    Vabo.Bind();                                                                         // bind vao & vbo
    Texture.Bind();                                                                      // bind texture
    mold::render::shader::SetUniform4fv("model", PositionMatrix);                        // give the shader our position matrix
    mold::render::shader::SetUniform4v("fcolour", glm::vec4(1.0f, 1.0f, 1.0f, Opacity)); // pass colour information needed for transparency
}

std::string mold::render::objects::GameObject::Type()
{
    return "Empty"; // empty game object
}

glm::vec3 mold::render::objects::GameObject::GetPosition()
{
    return PositionMatrix[3]; // position is stored on the 3rd axis (is this the correct term?)
}