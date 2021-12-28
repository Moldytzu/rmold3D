#include <rmold3D/mold.h>

uint mold::render::shader::CompileShader(const char *source, uint type)
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

    //clean up the objects as you will not use them
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}

bool mold::render::shader::GetCompilationError(uint shader)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    return (bool)success;
}

bool mold::render::shader::GetLinkError(uint program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success);
    return (bool)success;
}