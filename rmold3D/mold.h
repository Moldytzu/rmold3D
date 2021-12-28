#include <3rd-Party/glad/include/glad/glad.h>
#include <3rd-Party/glm/glm/glm.hpp>
#include <3rd-Party/glm/glm/gtc/matrix_transform.hpp>
#include <3rd-Party/glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <map>

#define Vertex(X, Y, Z) X, Y, Z
#define TexCoord(X, Y) X, Y

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
            uint GenerateTextureIndex(Texture texture);
        };

        namespace shader
        {
            uint CompileShader(const char *source, uint type);
            uint LinkShader(uint fragment, uint vertex);
            bool GetCompilationError(uint shader);
            bool GetLinkError(uint program);

            inline const char *VertexShaderSource = "#version 330 core\n"
                                                    "layout (location = 0) in vec3 vertexPosition;\n"
                                                    "layout (location = 1) in vec2 textureCoordornate;\n"
                                                    "out vec2 textureCoord;\n"
                                                    "uniform mat4 model;\n"
                                                    "uniform mat4 view;\n"
                                                    "uniform mat4 projection;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
                                                    "   textureCoord = textureCoordornate;\n"
                                                    "}\n";

            inline const char *FragmentShaderSource = "#version 330 core\n"
                                                      "out vec4 FragColor;\n"
                                                      "in vec2 textureCoord;\n"
                                                      "uniform sampler2D mainTexture;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "   FragColor = texture(mainTexture, textureCoord);\n"
                                                      "}\n";

            inline uint GlobalShaderProgram;
        };

        namespace vabo
        {
            struct VABO
            {
                uint VAO;
                uint VBO;
            };

            VABO GenerateVABO(float *vertices, size_t len);
        };

        namespace camera
        {
            inline glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            inline glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
            inline glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            inline float Yaw = -90.0f; //-90.0f left, 0 front, 90.0f right
            inline float Pitch = 0.0f; //90.0f up, 0 front, -90.0f down
        };

        namespace objects
        {
            class GameObject
            {
            public:
                GameObject();
                void Translate(glm::vec3 offset); //translate position
                void Move(glm::vec3 position);    //set position
                glm::vec3 GetPosition(); //get position
                virtual void Init();
                virtual void Draw();
                glm::mat4 PositionMatrix = glm::mat4(1.0f);
                bool Initialized = false;
                bool Enabled = false;

            protected:
                uint Texture = 0;
                mold::render::vabo::VABO Vabo;
                float *Vertices;
            };

            class Cube : public GameObject
            {
            public:
                Cube();
                Cube(mold::render::image::Texture texture);

                void Init(mold::render::image::Texture texture);
                void Draw();
            };

            class GameObjectsManager
            {
            public:
                void Add(const char *name, GameObject *object);
                void Remove(const char *name);
                GameObject *Get(const char *name);
                std::map<const char *, GameObject *> Get();

            private:
                std::map<const char *, GameObject *> GameObjects;
            };
        };
    };

    namespace settings
    {
        inline float FOV = 90.0f;
        inline float WindowWidth;
        inline float WindowHeight;
    };

    namespace time
    {
        inline float DeltaTime;
        inline float LastFrame;
    };

    enum EventType
    {
        Redraw = 1,
        Tick = 2,
    };

    class EventSystem
    {
    public:
        void AttachCallback(EventType type, void (*callback)());
        void DetachCallback(EventType type);
        std::map<EventType, void (*)()> GetMap();

    private:
        std::map<EventType, void (*)()> events; //type, callback
    };

    inline GLFWwindow *GlobalWindow;
    inline EventSystem GlobalEventSystem;
    inline mold::render::objects::GameObjectsManager GlobalGameObjects;

    bool Init(uint width, uint height);
    void Run();
};
