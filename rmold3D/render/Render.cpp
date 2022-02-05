#include <rmold3D/mold.h>

mold::render::Colour::Colour(uint8_t r, uint8_t g, uint8_t b)
{
    R = r, G = g, B = b; // set RGB values individually
}

mold::render::Colour::Colour(uint8_t rgb)
{
    R = G = B = rgb; // set RGB to the same value
}

void mold::render::DrawTriangles(uint count)
{
    glDrawArrays(GL_TRIANGLES, 0, count); // draw triangles
}