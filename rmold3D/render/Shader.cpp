#include <rmold3D/mold.h>

uint mold::render::shader::CompileShader(const char *source,uint type)
{
    unsigned int vertexShader = glCreateShader(type); //create and compile shader
    glShaderSource(vertexShader, 1, &source, NULL);
    glCompileShader(vertexShader);
    return vertexShader;
}

uint mold::render::shader::LinkShader(uint fragment, uint vertex)
{
    uint shaderProgram = glCreateProgram(); //link the shaders together to form a program
    glAttachShader(shaderProgram, vertex);
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram);
    return shaderProgram;
}