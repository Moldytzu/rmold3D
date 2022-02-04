#include <rmold3D/mold.h>

mold::render::Colour::Colour(uint8_t r, uint8_t g, uint8_t b) 
{
    R = r, G = g, B = b;
}

mold::render::Colour::Colour(uint8_t rgb) 
{
    R = G = B = rgb;
}

void mold::render::DrawTriangles(uint count)
{
    glDrawArrays(GL_TRIANGLES, 0, count);
}