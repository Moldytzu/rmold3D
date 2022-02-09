#include <rmold3D/mold.h>

void mold::render::shader::SetUniform4fv(std::string location, glm::mat4 matrix)
{
    uint loc = glGetUniformLocation(mold::render::shader::GlobalShaderProgram, location.c_str()); // get location
    glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);                                          // set matrix
}

void mold::render::shader::SetUniform4v(std::string location, glm::vec4 vector)
{
    uint loc = glGetUniformLocation(mold::render::shader::GlobalShaderProgram, location.c_str()); // get location
    glUniform4fv(loc, 1, &vector[0]);                                                             // set vector
}

void mold::render::shader::SetUniform3v(std::string location, glm::vec3 vector)
{
    uint loc = glGetUniformLocation(mold::render::shader::GlobalShaderProgram, location.c_str()); // get location
    glUniform4fv(loc, 1, &vector[0]);                                                             // set vector
}

uint mold::render::shader::CompileShader(std::string source, uint type)
{
    const char *cstr = source.c_str();
    unsigned int vertexShader = glCreateShader(type); // create and compile shader
    glShaderSource(vertexShader, 1, &cstr, NULL);     // specify the source
    glCompileShader(vertexShader);                    // compile
    return vertexShader;                              // return it
}

uint mold::render::shader::LinkShader(uint fragment, uint vertex)
{
    uint shaderProgram = glCreateProgram(); // create a program
    glAttachShader(shaderProgram, vertex);  // attach shaders
    glAttachShader(shaderProgram, fragment);
    glLinkProgram(shaderProgram); // link it

    // clean up the objects as you will not use them
    glDeleteShader(vertex);
    glDeleteShader(fragment);

    return shaderProgram;
}

bool mold::render::shader::GetCompilationError(uint shader)
{
    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // get compilation status
    return (bool)success;
}

bool mold::render::shader::GetLinkError(uint program)
{
    int success;
    glGetProgramiv(program, GL_LINK_STATUS, &success); // get link status
    return (bool)success;
}