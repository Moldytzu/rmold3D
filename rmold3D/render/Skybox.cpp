/*
    rmmold3D
    Copyright (C) 2022  Moldovan Alexandru

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#include "../mold.h"

using namespace mold::render;

mold::render::Skybox::Skybox() {}

mold::render::Skybox::Skybox(std::string up, std::string side, std::string down)
{

    upT = image::Texture(up);
    sideT = image::Texture(side);
    downT = image::Texture(down);

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

void mold::render::Skybox::Bind()
{
    glm::mat4 model;

    Vabo.Bind();                                 // bind vao & vbo
    GlobalShader.Bind();                         // bind the shader
    GlobalShader.Set("fcolour", glm::vec4(1.0)); // pass colour information needed for transparency

    // up
    model = glm::translate(glm::mat4(1.0f), render::camera::Position + glm::vec3(0, Scale / 2, 0)); // translate the model to the camera
    model = glm::scale(model, glm::vec3(Scale + 1, 1, Scale + 1));                                  // scale x and z
    upT.Bind();                                                                                     // bind texture
    GlobalShader.Set("model", model);                                                               // give the shader our position matrix
    DrawTriangles(6);                                                                               // draw 6 triangles

    // down
    model = glm::translate(glm::mat4(1.0f), render::camera::Position - glm::vec3(0, Scale / 2, 0)); // translate the model to the camera
    model = glm::scale(model, glm::vec3(Scale + 1, 1, Scale + 1));                                  // scale x and z
    downT.Bind();                                                                                   // bind texture
    GlobalShader.Set("model", model);                                                               // give the shader our position matrix
    DrawTriangles(6);                                                                               // draw 6 triangles

    // sides
    sideT.Bind(); // bind texture

    // front
    model = glm::translate(glm::mat4(1.0f), render::camera::Position - glm::vec3(0, 0, Scale / 2 - 1)); // translate the model to the camera
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(1, 0, 0)));                // flip it
    model = glm::scale(model, glm::vec3(Scale + 1, 1, Scale + 1));                                      // scale x and z
    GlobalShader.Set("model", model);                                                                   // give the shader our position matrix
    DrawTriangles(6);                                                                                   // draw 6 triangles

    // back
    model = glm::translate(glm::mat4(1.0f), render::camera::Position + glm::vec3(0, 0, Scale / 2 - 1)); // translate the model to the camera
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(1, 0, 0)));                // flip it
    model = glm::scale(model, glm::vec3(Scale + 1, 1, Scale + 1));                                      // scale x and z
    GlobalShader.Set("model", model);                                                                   // give the shader our position matrix
    DrawTriangles(6);                                                                                   // draw 6 triangles

    // right
    model = glm::translate(glm::mat4(1.0f), render::camera::Position + glm::vec3(Scale / 2, 0, 0)); // translate the model to the camera
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(1, 0, 0)));            // flip it
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0, 0, 1)));            // flip it
    model = glm::scale(model, glm::vec3(Scale + 1, 1, Scale + 1));                                  // scale x and z
    GlobalShader.Set("model", model);                                                               // give the shader our position matrix
    DrawTriangles(6);                                                                               // draw 6 triangles

    // left
    model = glm::translate(glm::mat4(1.0f), render::camera::Position - glm::vec3(Scale / 2, 0, 0)); // translate the model to the camera
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(1, 0, 0)));            // flip it
    model = glm::rotate(model, glm::radians(90.0f), glm::normalize(glm::vec3(0, 0, 1)));            // flip it
    model = glm::scale(model, glm::vec3(Scale + 1, 1, Scale + 1));                                  // scale x and z
    GlobalShader.Set("model", model);                                                               // give the shader our position matrix
    DrawTriangles(6);                                                                               // draw 6 triangles
}

void mold::render::Skybox::Deallocate()
{
}