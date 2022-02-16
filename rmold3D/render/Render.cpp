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

mold::render::Colour::Colour(uint8_t r, uint8_t g, uint8_t b) : R{r}, G{g}, B{b} {} // set RGB values individually
mold::render::Colour::Colour(uint8_t rgb) : R{rgb}, G{rgb}, B{rgb} {}               // set RGB to the same value

void mold::render::DrawTriangles(uint count)
{
    glDrawArrays(GL_TRIANGLES, 0, count); // draw triangles
}

void mold::render::HandleErrors()
{
    GLenum error = glGetError();
    switch (error)
    {
    case GL_INVALID_ENUM:
        log::Error("GL_INVALID_ENUM");
        break;
    case GL_INVALID_VALUE:
        log::Error("GL_INVALID_VALUE");
        break;
    case GL_INVALID_OPERATION:
        log::Error("GL_INVALID_OPERATION");
        break;
    case GL_INVALID_FRAMEBUFFER_OPERATION:
        log::Error("GL_INVALID_FRAMEBUFFER_OPERATION");
        break;
    case GL_OUT_OF_MEMORY:
        log::Fatal("GL_OUT_OF_MEMORY");
        break;
    default:
        break;
    }
}