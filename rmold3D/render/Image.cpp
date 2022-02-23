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

#ifdef _WIN32
#include <windows.h> //GetModuleFileNameW
#else
#include <limits.h>
#include <unistd.h> //readlink
#endif

using namespace mold::render::image;

std::unordered_map<std::string, mold::render::image::Texture> db; // database to store the filenames with their coresponding texture data

mold::render::image::Texture::Texture() {}

std::unordered_map<std::string, mold::render::image::Texture> mold::render::image::Texture::GetDB() // get database
{
    return db;
}

void mold::render::image::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, TextureIndex); // bind texture
}

void mold::render::image::Texture::Deallocate()
{
    delete[] PixelData;                 // delete pixel data as it's allocated on the heap
    glDeleteTextures(1, &TextureIndex); // delete texture
}

void mold::render::image::Texture::CreateIndex()
{
    glGenTextures(1, &TextureIndex);                                     // generate texture buffer
    glBindTexture(GL_TEXTURE_2D, TextureIndex);                          // bind texture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE); // clamp texture to edges
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR); // linear mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, PixelData); // give gpu the texture
    glGenerateMipmap(GL_TEXTURE_2D);                                                               // generate mipmap for it
}

mold::render::image::Texture::Texture(mold::render::Colour colour)
{
    // generate a solid colour 16x16 texture
    Width = 16;
    Height = 16;

    PixelData = new uint8_t[16 * 16 * 3];         // 16*16 texture with 3 bytes per pixel data
    for (int it = 0, idx = 0; it < 16 * 16; it++) // fill pixel data with R, G and B values
    {
        PixelData[idx++] = colour.R;
        PixelData[idx++] = colour.G;
        PixelData[idx++] = colour.B;
    }

    CreateIndex(); // generate index
}

mold::render::image::Texture::Texture(std::string filename)
{
    // don't load if the file is already in memory
    if (db.count(filename))
    {
        *this = db[filename];
        return;
    }

    // we support only bitmaps
    if (!EndsWith(filename, std::string(".bmp")))
    {
        log::Error("Unsupported texture format. Please use the 24-bit uncompressed bitmap.");
        *this = Texture(Colour(255)); // create white texture
        return;
    }

    std::ifstream stream(filename); // create stream

    if (!stream.good()) // fails if file doesn't exist
    {
        // try to append the program's path
        log::Error("Failed to load bitmap " + filename + " Trying to append the current directory");

#ifdef __WIN32__
        wchar_t path[MAX_PATH] = {0};
        GetModuleFileNameW(NULL, path, MAX_PATH);
        std::string execPath = std::string(path);
#else
        char result[PATH_MAX];
        ssize_t count = readlink("/proc/self/exe", result, PATH_MAX);
        std::string execPath = std::string(result, (count > 0) ? count : 0);
#endif

        execPath = execPath.substr(0,execPath.find_last_of("/\\")); // get only the directory

        stream = std::ifstream(execPath + "/" + filename);
        log::Info(execPath);
        if (!stream.good())
        {
            log::Error("Nevermind...");
            *this = Texture(Colour(255)); // create white texture
            return;
        }
    }

    BitmapImageHeader *header = new BitmapImageHeader; // allocate memory for the header

    stream.read((char *)header, sizeof(BitmapImageHeader)); // read it
    if (!stream.good())                                     // fail
    {
        log::Error("Failed to read header of bitmap " + filename);
        *this = Texture(Colour(255)); // create white texture
        return;
    }

    if (header->BPP != 24) // check bpp
    {
        log::Error("Unexpected " + std::to_string(header->BPP) + " bpp. Expected 24.");
        *this = Texture(Colour(255)); // create white texture
        return;
    }

    Width = header->BitmapWidth;
    Height = header->BitmapHeight;

    PixelData = new uint8_t[header->ImageSize]; // allocate memory for the pixel data

    stream.read((char *)PixelData, header->ImageSize); // read it
#ifndef __WIN32__
    if (!stream.good()) // fails on windows but not on linux
    {
        log::Error("Failed to read contents of bitmap " + filename);
        *this = Texture(Colour(255)); // create white texture
        return;
    }
#endif

    for (int i = 0; i < header->ImageSize; i += 3)
    {
        // reverse all of the colors. (bgr -> rgb)

        uint8_t temp = PixelData[i];

        PixelData[i] = PixelData[i + 2];

        PixelData[i + 2] = temp;
    }

    // delete header and close stream
    stream.close();
    delete header;

    CreateIndex(); // create index for opengl

    db.emplace(std::move(filename), std::move(*this)); // add to database
}