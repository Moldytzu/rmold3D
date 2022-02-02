#include <rmold3D/mold.h>

mold::render::image::Texture::Texture() {}

void mold::render::image::Texture::Bind()
{
    glBindTexture(GL_TEXTURE_2D, TextureIndex); // bind texture
}

mold::render::image::Texture::Texture(std::string filename)
{
    if (!filename.ends_with(".bmp"))
    {
        mold::log::Error("Unsupported format. Please use the .BMP format.");
        return;
    }

    FILE *file = fopen(filename.c_str(), "rb"); // open file
    uint32_t size;
    uint16_t bitPlanes;
    uint16_t bpp;

    if ((uint64_t)file == 0)
    {
        mold::log::Error("Failed to load bitmap " + filename);
        return;
    }

    mold::render::image::BitmapImageHeader *header = new mold::render::image::BitmapImageHeader;

    if (fread(header, sizeof(mold::render::image::BitmapImageHeader), 1, file) != 1)
    {
        mold::log::Error("Failed to read header of bitmap " + filename);
        return;
    }

    if (header->BPP != 24)
    {
        mold::log::Error("Unexpected " + std::to_string(header->BPP) + " bpp. Expected 24.");
        return;
    }

    Width = header->BitmapWidth;
    Height = header->BitmapHeight;

    PixelData = new uint8_t[header->ImageSize];

    if (fread(PixelData, header->ImageSize, 1, file) != 1)
    {
        mold::log::Error("Failed to read contents of bitmap " + filename);
        return;
    }

    for (int i = 0; i < header->ImageSize; i += 3)
    { // reverse all of the colors. (bgr -> rgb)

        uint8_t temp = PixelData[i];

        PixelData[i] = PixelData[i + 2];

        PixelData[i + 2] = temp;
    }

    glGenTextures(1, &TextureIndex);
    glBindTexture(GL_TEXTURE_2D, TextureIndex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, Width, Height, 0, GL_RGB, GL_UNSIGNED_BYTE, PixelData);
    glGenerateMipmap(GL_TEXTURE_2D);
}