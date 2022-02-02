#include <rmold3D/mold.h>

mold::render::objects::Cube::Cube() {}
mold::render::objects::Cube::Cube(mold::render::image::Texture texture) { Init(texture); }

void mold::render::objects::Cube::Init(mold::render::image::Texture texture)
{
    float vertices[] = {
        Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f), // back
        Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 0.0f),
        Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),
        Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),
        Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 0.0f),

        Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f), // front
        Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),
        Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),
        Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 1.0f),
        Vertex(-0.5f, 0.5f, 0.5f), TexCoord(0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f),

        Vertex(-0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f), // left
        Vertex(-0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),
        Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f),
        Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f),
        Vertex(-0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f),

        Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f), // right
        Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),
        Vertex(0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f),
        Vertex(0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f),
        Vertex(0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f),
        Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f),

        Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f), // bottom
        Vertex(0.5f, -0.5f, -0.5f), TexCoord(1.0f, 1.0f),
        Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),
        Vertex(0.5f, -0.5f, 0.5f), TexCoord(1.0f, 0.0f),
        Vertex(-0.5f, -0.5f, 0.5f), TexCoord(0.0f, 0.0f),
        Vertex(-0.5f, -0.5f, -0.5f), TexCoord(0.0f, 1.0f),

        Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f), // up
        Vertex(0.5f, 0.5f, -0.5f), TexCoord(1.0f, 1.0f),
        Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f),
        Vertex(0.5f, 0.5f, 0.5f), TexCoord(1.0f, 0.0f),
        Vertex(-0.5f, 0.5f, 0.5f), TexCoord(0.0f, 0.0f),
        Vertex(-0.5f, 0.5f, -0.5f), TexCoord(0.0f, 1.0f)};

    if (Initialized) // if initialized just retun
        return;

    Initialized = Enabled = true; // enable and initialize
    Texture = texture; // set texture
    Vabo = mold::render::VABO(vertices, sizeof(vertices)); // generate VBO & VAO
}

void mold::render::objects::Cube::Draw()
{
    Vabo.Bind(); // bind vao & vbo
    Texture.Bind(); // bind texture
    glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "model"), 1, GL_FALSE, &PositionMatrix[0][0]); //give the shader our position
    glDrawArrays(GL_TRIANGLES, 0, 36); //draw triangles
}

std::string mold::render::objects::Cube::Type()
{
    return "Textured Cube";
}