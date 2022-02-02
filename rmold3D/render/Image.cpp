#include <rmold3D/mold.h>

mold::render::image::Texture mold::render::image::LoadRGBBitmap(std::string filename)
{
    FILE *file;
    uint32_t size;
    uint16_t bitPlanes;
    uint16_t bpp;

    file = fopen(filename.c_str(), "rb");

    if ((uint64_t)file == 0)
    {
        char buffer[1024];
        sprintf(buffer, "Couldn't find the bitmap image %s!", filename);

        puts(buffer);

        return {};
    }

    mold::render::image::BitmapImageHeader *header = (mold::render::image::BitmapImageHeader *)malloc(sizeof(mold::render::image::BitmapImageHeader));
    if (fread(header, sizeof(mold::render::image::BitmapImageHeader), 1, file) != 1)
        return {};

    if (header->BPP != 24)
        return {};

    mold::render::image::Texture texture;

    texture.Width = header->BitmapWidth;
    texture.Height = header->BitmapHeight;

    texture.PixelData = (uint8_t *)malloc(header->ImageSize);

    if (fread(texture.PixelData, header->ImageSize, 1, file) != 1)
        return {};

    for (int i = 0; i < header->ImageSize; i += 3)
    { // reverse all of the colors. (bgr -> rgb)

        uint8_t temp = texture.PixelData[i];

        texture.PixelData[i] = texture.PixelData[i + 2];

        texture.PixelData[i + 2] = temp;
    }

    return texture;
}

uint mold::render::image::GenerateTextureIndex(mold::render::image::Texture texture)
{
    uint t;
    glGenTextures(1, &t);
    glBindTexture(GL_TEXTURE_2D, t);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.Width, texture.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture.PixelData);
    glGenerateMipmap(GL_TEXTURE_2D);
    return t;
}