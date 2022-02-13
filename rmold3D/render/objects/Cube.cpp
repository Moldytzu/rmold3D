#include "../../mold.h"

using namespace mold::render::objects;

mold::render::objects::Cube::Cube() {}

mold::render::objects::Cube::Cube(image::Texture texture) : GameObject(texture)
{
    float vertices[] =
        {
            // back face
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // right up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 0.0f),  // left up
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),   // left down
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),   // left down
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f),  // right down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // right up

            // front face
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f), // right up
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),  // left up
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),   // left down
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),   // left down
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(0.0f, 1.0f),  // right down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f), // right up

            // left face
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),   // right up
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f),  // left up
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // left down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // left down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),  // right down
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),   // right up

            // right face
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),   // right up
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f),  // left up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // left down
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // left down
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),  // right down
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),   // right up

            // bottom face
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), // right up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 1.0f),  // left up
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),   // left down
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),   // left down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f),  // right down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), // right up

            // up face
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f),  // right up
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),   // left up
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f),    // left down
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f),    // left down
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(0.0f, 0.0f),   // right down
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f),}; // right up

    Vabo = VABO(vertices, sizeof(vertices)); // generate VBO & VAO
}

void mold::render::objects::Cube::Draw()
{
    DrawTriangles(36); // draw 36 triangles
}

std::string mold::render::objects::Cube::Type()
{
    return "Textured Cube";
}