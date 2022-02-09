#include <rmold3D/mold.h>

mold::render::objects::Cube::Cube() {}

mold::render::objects::Cube::Cube(mold::render::image::Texture texture) : GameObject(texture)
{
    float vertices[] =
        {
            // back face
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // right up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),  // left up
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // left down
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // left down
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0),  // right down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // right up

            // front face
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // right up
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),  // left up
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // left down
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // left down
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0),  // right down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // right up

            // left face
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // right up
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0),  // left up
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // left down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // left down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),  // right down
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // right up

            // right face
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // right up
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0),  // left up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // left down
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0), // left down
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),  // right down
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // right up

            // bottom face
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0), // right up
            Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),  // left up
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),   // left down
            Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),   // left down
            Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0),  // right down
            Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0), // right up

            // up face
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0),  // right up
            Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f), Normal(0,-1,0),   // left up
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),    // left down
            Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f), Normal(0,-1,0),    // left down
            Vertex(-0.5f, 0.5f, 0.5f), TexCoord(0.0f, 0.0f), Normal(0,-1,0),   // right down
            Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f), Normal(0,-1,0),}; // right up

    Vabo = mold::render::VABO(vertices, sizeof(vertices)); // generate VBO & VAO
}

void mold::render::objects::Cube::Draw()
{
    mold::render::DrawTriangles(36); // draw 36 triangles
}

std::string mold::render::objects::Cube::Type()
{
    return "Textured Cube";
}