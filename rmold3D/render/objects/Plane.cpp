#include "../../mold.h"

using namespace mold::render::objects;

mold::render::objects::Plane::Plane() {}

mold::render::objects::Plane::Plane(image::Texture texture) : GameObject(texture)
{
    float vertices[] =
        {
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), // right up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 1.0f),  // left up
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),   // left down
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),   // left down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f),  // right down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), // right up
        };

    Vabo = mold::render::VABO(vertices, sizeof(vertices)); // generate VBO & VAO
}

void mold::render::objects::Plane::Draw()
{
    DrawTriangles(6); // draw 6 triangles
}

std::string mold::render::objects::Plane::Type()
{
    return "Textured Plane";
}