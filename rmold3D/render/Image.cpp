#include <rmold3D/mold.h>

mold::render::image::Texture::Texture() {}

void mold::render::image::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, TextureIndex); // bind texture
}

void mold::render::image::Texture::Deallocate()
{
    glDeleteTextures(1, &TextureIndex); // delete texture
}

void mold::render::image::Texture::CreateIndex()
{
    glGenTextures(1, &TextureIndex);
    glBindTexture(GL_TEXTURE_2D, TextureIndex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, PixelData);
    glGenerateMipmap(GL_TEXTURE_2D);
}

mold::render::image::Texture::Texture(mold::render::Colour colour)
{
    // generate a solid colour 16x16 texture
    Width = 16;
    Height = 16;

    PixelData = new uint8_t[16*16*3]; //16*16 texture with 3 bytes per pixel data
    for(int it = 0, idx = 0; it < 16*16; it++)
        {
            PixelData[idx++] = colour.R;
            PixelData[idx++] = colour.G;
            PixelData[idx++] = colour.B;
        }

    CreateIndex(); // generate index
}

mold::render::image::Texture::Texture(std::string filename)
{
    if (!filename.ends_with(".bmp"))
        {
            mold::log::Error("Unsupported texture format. Please use the 24-bit uncompressed bitmap.");
            mold::Destroy();
        }

    std::ifstream stream(filename);

    if (!stream.good())
        {
            mold::log::Error("Failed to load bitmap " + filename);
            mold::Destroy();
        }

    mold::render::image::BitmapImageHeader *header = new mold::render::image::BitmapImageHeader;

    stream.read((char*)header,sizeof(mold::render::image::BitmapImageHeader));
    if (!stream.good())
        {
            mold::log::Error("Failed to read header of bitmap " + filename);
            mold::Destroy();
        }

    if (header->BPP != 24)
        {
            mold::log::Error("Unexpected " + std::to_string(header->BPP) + " bpp. Expected 24.");
            mold::Destroy();
        }

    Width = header->BitmapWidth;
    Height = header->BitmapHeight;

    PixelData = new uint8_t[header->ImageSize];

    stream.read((char*)PixelData,header->ImageSize);
    if (!stream.good())
        {
            mold::log::Error("Failed to read contents of bitmap " + filename);
            mold::Destroy();
        }

    for (int i = 0; i < header->ImageSize; i += 3)
        {
            // reverse all of the colors. (bgr -> rgb)

            uint8_t temp = PixelData[i];

            PixelData[i] = PixelData[i + 2];

            PixelData[i + 2] = temp;
        }

    CreateIndex(); // create index for opengl
}