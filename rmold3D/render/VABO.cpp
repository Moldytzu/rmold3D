#include <rmold3D/mold.h>

mold::render::vabo::VABO mold::render::vabo::GenerateVABO(float *vertices, size_t len)
{
    uint VAO,VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                                              //tell opengl we want to use this vbo
    glBufferData(GL_ARRAY_BUFFER, len, vertices, GL_STATIC_DRAW);                       //set data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)0);                   //position attribute
    glEnableVertexAttribArray(0);                                                                    //enable
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void *)(3 * sizeof(float))); //texture coord attribute
    glEnableVertexAttribArray(1);                                                                    //enable
    return {VAO,VBO};
}