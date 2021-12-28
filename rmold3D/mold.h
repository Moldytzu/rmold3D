#include <3rd-Party/glad/include/glad/glad.h>
#include <3rd-Party/glm/glm/glm.hpp>
#include <3rd-Party/glm/glm/gtc/matrix_transform.hpp>
#include <3rd-Party/glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

namespace mold
{
    namespace render
    {
        namespace image
        {
            struct Texture
            {
                uint32_t Size;
                uint32_t Width;
                uint32_t Height;

                uint8_t *PixelData;
            };

            struct BitmapImageHeader //https://en.wikipedia.org/wiki/BMP_file_format
            {
                //bmp header
                uint8_t Signature1; //Should be B
                uint8_t Signature2; //Should be M

                uint32_t Size; //bmp file size

                uint32_t Reserved;

                uint32_t DataOffset; //offset to the image data

                //BITMAPINFOHEADER
                uint32_t HeaderSize;          //size of BITMAPINFOHEADER, always 0x28 (decimal 40)
                int32_t BitmapWidth;          //width of the image
                int32_t BitmapHeight;         //height of the image
                uint16_t Planes;              //bit planes
                uint16_t BPP;                 //bits per pixel
                uint32_t CompressionMethod;   //the method of compression used
                uint32_t ImageSize;           //size in bytes of the whole image, without headers
                int32_t HorizontalResolution; //resolution in pixel per metre
                int32_t VerticalResolution;   //resolution in pixel per metre
                uint32_t Colors;              //colors in the color palette
                uint32_t ImportantColors;     //important colors used, generally ignored
            } __attribute__((packed));

            Texture LoadRGBBitmap(const char *filename);
        };
    };
};
