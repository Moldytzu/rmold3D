#include "../mold.h"

using namespace mold::render;

mold::render::VABO::VABO() {}

mold::render::VABO::VABO(float *vertices, size_t len)
{
    // allocate vao and vbo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);                                                                          // bind vao
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                              // bind vbo
    glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);                                    // set data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);                   // position attribute
    glEnableVertexAttribArray(0);                                                                    // enable it
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))); // texture coord attribute
    glEnableVertexAttribArray(1);                                                                    // enable it
}

void mold::render::VABO::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, VBO); // bind vbo
    glBindVertexArray(VAO);             // bind vao
}

void mold::render::VABO::Deallocate()
{
    glDeleteBuffers(1, &VBO);      // delete vbo
    glDeleteVertexArrays(1, &VAO); // delete vao
}