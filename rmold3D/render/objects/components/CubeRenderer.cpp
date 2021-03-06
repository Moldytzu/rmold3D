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

#include "../../../mold.h"

using namespace mold::render::objects;

void CubeRenderer::Start()
{
    Public["Texture"] = mold::render::image::Texture(mold::render::Colour(255));

    float vertices[] =
        {
            // back face
            Vertex(-0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 0.0f), // right up
            Vertex(0.5f, -0.5f, -0.5f),
            TexCoord(1.0f, 0.0f), // left up
            Vertex(0.5f, 0.5f, -0.5f),
            TexCoord(1.0f, 1.0f), // left down
            Vertex(0.5f, 0.5f, -0.5f),
            TexCoord(1.0f, 1.0f), // left down
            Vertex(-0.5f, 0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // right down
            Vertex(-0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 0.0f), // right up

            // front face
            Vertex(-0.5f, -0.5f, 0.5f),
            TexCoord(0.0f, 0.0f), // right up
            Vertex(0.5f, -0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // left up
            Vertex(0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 1.0f), // left down
            Vertex(0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 1.0f), // left down
            Vertex(-0.5f, 0.5f, 0.5f),
            TexCoord(0.0f, 1.0f), // right down
            Vertex(-0.5f, -0.5f, 0.5f),
            TexCoord(0.0f, 0.0f), // right up

            // left face
            Vertex(-0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 1.0f), // right up
            Vertex(-0.5f, 0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // left up
            Vertex(-0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 0.0f), // left down
            Vertex(-0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 0.0f), // left down
            Vertex(-0.5f, -0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // right down
            Vertex(-0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 1.0f), // right up

            // right face
            Vertex(0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 1.0f), // right up
            Vertex(0.5f, 0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // left up
            Vertex(0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 0.0f), // left down
            Vertex(0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 0.0f), // left down
            Vertex(0.5f, -0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // right down
            Vertex(0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 1.0f), // right up

            // bottom face
            Vertex(-0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // right up
            Vertex(0.5f, -0.5f, -0.5f),
            TexCoord(1.0f, 1.0f), // left up
            Vertex(0.5f, -0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // left down
            Vertex(0.5f, -0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // left down
            Vertex(-0.5f, -0.5f, 0.5f),
            TexCoord(0.0f, 0.0f), // right down
            Vertex(-0.5f, -0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // right up

            // up face
            Vertex(-0.5f, 0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // right up
            Vertex(0.5f, 0.5f, -0.5f),
            TexCoord(1.0f, 1.0f), // left up
            Vertex(0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // left down
            Vertex(0.5f, 0.5f, 0.5f),
            TexCoord(1.0f, 0.0f), // left down
            Vertex(-0.5f, 0.5f, 0.5f),
            TexCoord(0.0f, 0.0f), // right down
            Vertex(-0.5f, 0.5f, -0.5f),
            TexCoord(0.0f, 1.0f), // right up
        };

    Vabo = mold::render::VABO(vertices, sizeof(vertices)); // generate VBO & VAO
}

void CubeRenderer::Tick()
{
    if(!camera::InView(Parent->GetPosition())) return;              // if it's not in view just return
    Parent->Bind();                                                 // bind matrices
    Vabo.Bind();                                                    // bind vertex info
    GetAny(Public["Texture"], mold::render::image::Texture).Bind(); // bind texture
    mold::render::DrawTriangles(36);                                // draw 36 triangles
}