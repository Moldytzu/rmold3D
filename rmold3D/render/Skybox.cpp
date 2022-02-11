#include "../mold.h"

using namespace mold::render;

mold::render::Skybox::Skybox() {}

mold::render::Skybox::Skybox(std::string up, std::string side, std::string down)
{
    Enabled = true;

    // add the sources and compile the shader
    SkyShader.AttachSource(SkyboxVertexSource, GL_VERTEX_SHADER);
    SkyShader.AttachSource(SkyboxFragmentSource, GL_FRAGMENT_SHADER);
    SkyShader.Recompile();

    // create vao and vbo
    float vertices[] = {
        -1.0f, 1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, -1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f,

        -1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, 1.0f,
        -1.0f, 1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f, 1.0f,
        1.0f, -1.0f, 1.0f};

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);

    // create texture
    glGenTextures(1, &TextureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);

    image::Texture upT = image::Texture(up);
    image::Texture sideT = image::Texture(side);
    image::Texture downT = image::Texture(down);

    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, GL_RGB, upT.Width, upT.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, upT.PixelData);       // top
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, GL_RGB, downT.Width, downT.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, downT.PixelData); // bottom
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, GL_RGB, sideT.Width, sideT.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, sideT.PixelData); // right
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, GL_RGB, sideT.Width, sideT.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, sideT.PixelData); // left
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, GL_RGB, sideT.Width, sideT.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, sideT.PixelData); // back
    glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, GL_RGB, sideT.Width, sideT.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, sideT.PixelData); // front

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    SkyShader.Set("skybox", TextureID);
}

void mold::render::Skybox::Bind()
{
    if (!Enabled)
        return;
    glDepthMask(GL_FALSE);
    SkyShader.Bind();                                                                                                                                         // bind the shader
    SkyShader.Set("view", glm::mat4(glm::mat3(glm::lookAt(render::camera::Position, render::camera::Position + render::camera::Front, render::camera::Up)))); // set camera view
    SkyShader.Set("projection", glm::perspective(glm::radians(settings::FOV), settings::WindowWidth / settings::WindowHeight, 0.1f, 100.0f));                 // set perspective
    glBindVertexArray(VAO);                                                                                                                                   // bind vao
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                                                                                       // bind vbo
    glBindTexture(GL_TEXTURE_CUBE_MAP, TextureID);                                                                                                            // bind texture
    DrawTriangles(36);                                                                                                                                        // draw
    glDepthMask(GL_TRUE);
}

void mold::render::Skybox::Deallocate()
{
    if (!Enabled)
        return;
    SkyShader.Deallocate();
}