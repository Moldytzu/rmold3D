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
    if (!filename.ends_with(".bmp")) // we support only bitmaps
    {
        mold::log::Error("Unsupported texture format. Please use the 24-bit uncompressed bitmap.");
        mold::Destroy();
    }

    std::ifstream stream(filename); // create stream

    if (!stream.good()) // fails if file doesn't exist
    {
        mold::log::Error("Failed to load bitmap " + filename);
        mold::Destroy();
    }

    mold::render::image::BitmapImageHeader *header = new mold::render::image::BitmapImageHeader; // allocate memory for the header

    stream.read((char *)header, sizeof(mold::render::image::BitmapImageHeader)); // read it
    if (!stream.good())                                                          // fail
    {
        mold::log::Error("Failed to read header of bitmap " + filename);
        mold::Destroy();
    }

    if (header->BPP != 24) // check bpp
    {
        mold::log::Error("Unexpected " + std::to_string(header->BPP) + " bpp. Expected 24.");
        mold::Destroy();
    }

    Width = header->BitmapWidth;
    Height = header->BitmapHeight;

    PixelData = new uint8_t[header->ImageSize]; // allocate memory for the pixel data

    stream.read((char *)PixelData, header->ImageSize); // read it
    if (!stream.good())                                // fail
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

    // delete header and close stream
    stream.close();
    delete header;

    CreateIndex(); // create index for opengl
}