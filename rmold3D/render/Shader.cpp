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

mold::render::Shader::Shader() : Shaders{new uint[0xFFFF]} // 64k shaders should be enough :)
{
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
    delete[] Shaders;
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
    {
        GLint maxLength = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        // The maxLength includes the NULL character
        char *errorLog = new char[maxLength];
        glGetShaderInfoLog(shader, maxLength, &maxLength, &errorLog[0]);
        errorLog[maxLength - 1] = 0;
        log::Error(errorLog);
        delete errorLog;
        if (type == GL_FRAGMENT_SHADER)
            log::Fatal("Failed to attach fragemnt shader");
        else if (type == GL_VERTEX_SHADER)
            log::Fatal("Failed to attach vertex shader");
        else
            log::Fatal("Failed to attach a shader with an unknown type");
    }

    Shaders[ShadersIndex++] = shader;
}

void mold::render::Shader::Set(std::string location, glm::mat4 matrix)
{
    uint loc = glGetUniformLocation(Program, location.c_str());   // get location
    glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(matrix)); // set matrix
}

void mold::render::Shader::Set(std::string location, glm::vec4 vector)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform4fv(loc, 1, glm::value_ptr(vector));               // set vector
}

void mold::render::Shader::Set(std::string location, glm::vec3 vector)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform3fv(loc, 1, glm::value_ptr(vector));               // set vector
}

void mold::render::Shader::Set(std::string location, int value)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform1i(loc, value);                                    // set value
}

void mold::render::Shader::Set(std::string location, float value)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform1f(loc, value);                                    // set value
}

void mold::render::Shader::Set(std::string location, bool value)
{
    uint loc = glGetUniformLocation(Program, location.c_str()); // get location
    glUniform1i(loc, value);                                    // set value
}