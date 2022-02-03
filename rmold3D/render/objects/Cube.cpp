#include <rmold3D/mold.h>

mold::render::objects::Cube::Cube() {}
mold::render::objects::Cube::Cube(mold::render::image::Texture texture) { Init(texture); }

void mold::render::objects::Cube::Init(mold::render::image::Texture texture)
{
    float vertices[] = {
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
        Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f)}; // right up

    if (Initialized) // if initialized just retun
        return;

    Initialized = Enabled = true;                          // enable and initialize
    Texture = texture;                                     // set texture
    Vabo = mold::render::VABO(vertices, sizeof(vertices)); // generate VBO & VAO
}

void mold::render::objects::Cube::Draw()
{
    Vabo.Bind();                                                                              // bind vao & vbo
    Texture.Bind();                                                                           // bind texture
    mold::render::shader::SetUniform4fv("model", PositionMatrix);                             // give the shader our position matrix
    mold::render::shader::SetUniform4v("fcolour", glm::vec4(1.0f, 1.0f, 1.0f, Opacity)); // pass colour information needed for transparency
    mold::render::DrawTriangles(36);                                                          //draw 36 triangles
}

std::string mold::render::objects::Cube::Type()
{
    return "Textured Cube";
}