#include <rmold3D/mold.h>

mold::render::objects::Cube::Cube() {}
mold::render::objects::Cube::Cube(mold::render::image::Texture texture) { Init(texture); }

void mold::render::objects::Cube::Init(mold::render::image::Texture texture)
{
    if (Initialized)
        return;
    Initialized = true;
    Enabled = true;
    Texture = mold::render::image::GenerateTextureIndex(texture);

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

    Vabo = mold::render::vabo::GenerateVABO(vertices, sizeof(vertices));
}

void mold::render::objects::Cube::Draw()
{
    glUniformMatrix4fv(glGetUniformLocation(mold::render::shader::GlobalShaderProgram, "model"), 1, GL_FALSE, &PositionMatrix[0][0]); //give the shader our position
    glBindTexture(GL_TEXTURE_2D, Texture);                                                                                            //set the texture
    glBindBuffer(GL_ARRAY_BUFFER, Vabo.VBO);                                                                                          //vbo of the cube
    glBindVertexArray(Vabo.VAO);                                                                                                      //vao of the cube
    glDrawArrays(GL_TRIANGLES, 0, 36);
}

std::string mold::render::objects::Cube::Type()
{
    return "Textured Cube";
}