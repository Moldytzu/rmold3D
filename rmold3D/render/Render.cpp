#include <rmold3D/mold.h>

void mold::render::DrawTriangles(uint count)
{
    glDrawArrays(GL_TRIANGLES, 0, count);
}