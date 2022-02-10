#include <rmold3D/mold.h>

using namespace mold::render;

mold::render::Shader::Shader()
{
    Shaders = new uint[0xFFFF]; // 64k shaders should be enough :)
}

void mold::render::Shader::Recompile()
{
    Program = glCreateProgram();           // create new program
    for (int i = 0; i < ShadersIndex; i++) // attach each shader
        glAttachShader(Program, Shaders[i]);
    glLinkProgram(Program); // link the program

    int success;
    glGetProgramiv(Program, GL_LINK_STATUS, &success); // get compilation status
    if (!success)
        log::Fatal("Failed to link shader.");
}

void mold::render::Shader::Deallocate()
{
    glDeleteProgram(Program);
    delete Shaders;
}

void mold::render::Shader::Bind()
{
    if (Program != 0xdeadbeef)
        glUseProgram(Program);
}

void mold::render::Shader::AttachSource(std::string source, uint type)
{
    const char *cstr = source.c_str();
    unsigned int shader = glCreateShader(type); // create and compile shader
    glShaderSource(shader, 1, &cstr, NULL);     // specify the source
    glCompileShader(shader);                    // compile

    int success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success); // get compilation status
    if (!success)
        log::Fatal("Failed to attach shader with the type " + std::to_string(type));

    Shaders[ShadersIndex++] = shader;
}

void mold::render::Shader::Set(std::string location, glm::mat4 matrix)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniformMatrix4fv(loc, 1, GL_FALSE, &matrix[0][0]);        // set matrix
}

void mold::render::Shader::Set(std::string location, glm::vec4 vector)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform4fv(loc, 1, &vector[0]);                           // set vector
}

void mold::render::Shader::Set(std::string location, glm::vec3 vector)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform4fv(loc, 1, &vector[0]);                           // set vector
}