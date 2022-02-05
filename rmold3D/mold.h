#include <3rd-Party/glad/include/glad/glad.h>
#include <3rd-Party/glm/glm/glm.hpp>
#include <3rd-Party/glm/glm/gtc/matrix_transform.hpp>
#include <3rd-Party/glm/glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>
#include <unordered_map>
#include <stdarg.h>
#include <chrono>
#include <vector>
#include <string>
#include <memory>
#include <fstream>

#define Vertex(X, Y, Z) X, Y, Z
#define TexCoord(X, Y) X, Y

namespace mold
{
    enum EventType
    {
        Redraw,
        Tick,
        Resize,
        Exit,
        Mouse,
        LAST,
    };

    namespace render
    {
        class Colour
        {
        public:
            Colour(uint8_t rgb);
            Colour(uint8_t r, uint8_t g, uint8_t b);
            uint8_t R, G, B;
        };

        namespace image
        {
            class Texture
            {
            public:
                Texture();
                Texture(std::string filename);
                Texture(mold::render::Colour colour);
                void Bind();
                void Deallocate();

                uint32_t Size;
                uint32_t Width;
                uint32_t Height;
                uint8_t *PixelData;

            private:
                void CreateIndex();
                uint TextureIndex;
            };

            struct BitmapImageHeader // https://en.wikipedia.org/wiki/BMP_file_format
            {
                // bmp header
                uint8_t Signature1; // Should be B
                uint8_t Signature2; // Should be M

                uint32_t Size; // bmp file size

                uint32_t Reserved;

                uint32_t DataOffset; // offset to the image data

                // BITMAPINFOHEADER
                uint32_t HeaderSize;          // size of BITMAPINFOHEADER, always 0x28 (decimal 40)
                int32_t BitmapWidth;          // width of the image
                int32_t BitmapHeight;         // height of the image
                uint16_t Planes;              // bit planes
                uint16_t BPP;                 // bits per pixel
                uint32_t CompressionMethod;   // the method of compression used
                uint32_t ImageSize;           // size in bytes of the whole image, without headers
                int32_t HorizontalResolution; // resolution in pixel per metre
                int32_t VerticalResolution;   // resolution in pixel per metre
                uint32_t Colors;              // colors in the color palette
                uint32_t ImportantColors;     // important colors used, generally ignored
            } __attribute__((packed));
        };

        namespace shader
        {
            void SetUniform4fv(std::string location, glm::mat4 matrix);
            void SetUniform4v(std::string location, glm::vec4 vector);

            uint CompileShader(std::string source, uint type);
            uint LinkShader(uint fragment, uint vertex);
            bool GetCompilationError(uint shader);
            bool GetLinkError(uint program);

            inline std::string VertexShaderSource = "#version 330 core\n"
                                                    "layout (location = 0) in vec3 vertexPosition;\n"
                                                    "layout (location = 1) in vec2 textureCoordornate;\n"
                                                    "out vec2 textureCoord;\n"
                                                    "out vec4 icolour;\n"
                                                    "uniform mat4 model;\n"
                                                    "uniform mat4 view;\n"
                                                    "uniform mat4 projection;\n"
                                                    "uniform vec4 fcolour;\n"
                                                    "void main()\n"
                                                    "{\n"
                                                    "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
                                                    "   textureCoord = textureCoordornate;\n"
                                                    "   icolour = fcolour;\n"
                                                    "}\n";

            inline std::string FragmentShaderSource = "#version 330 core\n"
                                                      "out vec4 FragColor;\n"
                                                      "in vec2 textureCoord;\n"
                                                      "in vec4 icolour;\n"
                                                      "uniform sampler2D mainTexture;\n"
                                                      "void main()\n"
                                                      "{\n"
                                                      "   FragColor = texture(mainTexture, textureCoord) * icolour;\n"
                                                      "}\n";

            inline uint GlobalShaderProgram;
        };

        class VABO
        {
        public:
            VABO();
            VABO(float *vertices, size_t len);

            void Bind();
            void Deallocate();

        private:
            uint VAO, VBO;
        };

        enum CameraDirection
        {
            Forward,
            Backwards,
            Left,
            Right
        };

        namespace camera
        {
            inline glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            inline glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
            inline glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            inline float Yaw = -90.0f; //-90.0f left, 0 front, 90.0f right
            inline float Pitch = 0.0f; // 90.0f up, 0 front, -90.0f down

            void Translate(CameraDirection direction, float value);
            void Rotate(CameraDirection direction, float value);
        };

        namespace objects
        {
            class GameObject;

            class Component
            {
            public:
                virtual void Tick();
                virtual void Start(GameObject *parent);
                virtual void Handle(mold::EventType event);

                bool Enabled = true;

            protected:
                GameObject *Parent = NULL;
            };

            class GameObject
            {
            public:
                ~GameObject();

                GameObject();
                GameObject(mold::render::image::Texture texture);

                void Translate(glm::vec3 offset);                             // translate position
                void Move(glm::vec3 position);                                // set position
                void Scale(glm::vec3 scaleFactor);                            // set scale factor
                void Rotate(glm::vec3 axis, float angle);                     // set rotation
                void ReplaceTexture(mold::render::image::Texture newTexture); // replace the texture with a new one
                void Bind();                                                  // bind everything
                glm::vec3 GetPosition();                                      // get position

                void AttachComponent(std::string name, Component *component);              // attach component
                void DettachComponent(std::string name);                                   // dettach component
                bool ExistsComponent(std::string name);                                    // check if component exists
                void TickComponents();                                                     // tick every component
                void HandleComponents(mold::EventType event);                              // forward events to every component
                std::unordered_map<std::string, mold::render::objects::Component *> GetComponents(); // get components attached

                virtual void Draw();
                virtual std::string Type();

                glm::mat4 PositionMatrix = glm::mat4(1.0f);

                bool Enabled = false;

                float Opacity = 1.0f;

            protected:
                mold::render::image::Texture Texture;
                mold::render::VABO Vabo;
                float *Vertices;
            };

            class Cube : public GameObject
            {
            public:
                Cube();
                Cube(mold::render::image::Texture texture);

                void Draw();
                std::string Type();
            };

            class GameObjectsManager
            {
            public:
                void Instantiate(GameObject *object);
                void Instantiate(GameObject *object, std::string name);
                void Remove(std::string name);
                bool Exists(std::string name);
                GameObject *Get(std::string name);
                std::unordered_map<std::string, GameObject *> Get();

            private:
                void Add(std::string name, GameObject *object);
                std::unordered_map<std::string, GameObject *> GameObjects;
            };
        };
        void DrawTriangles(uint count);
    };

    namespace settings
    {
        inline float FOV = 90.0f;
        inline float WindowWidth;
        inline float WindowHeight;
        inline float MouseSensibility = 0.5f;
    };

    namespace time
    {
        inline float DeltaTime;
        inline float LastFrame;
    };

    enum CursorLockingMode
    {
        Locked,
        Hidden,
        Normal,
        Centred,
        Wrapped,
    };

    namespace input
    {
        bool GetKey(int key);

        inline CursorLockingMode GlobalCursorLockMode;
        inline glm::vec2 GlobalCursorAxis;
        inline glm::vec2 GlobalCursorPos;
        inline float GlobalScrollAxis;
    };

    namespace log
    {
        void Info(std::string str);
        void Warn(std::string str);
        void Error(std::string str);
    };

    class EventSystem
    {
    public:
        void AttachCallback(EventType type, void (*callback)());
        void DetachCallback(EventType type);

        bool ExistsCallback(EventType type);

        void CallEvent(EventType type);

        std::unordered_map<EventType, void (*)()> GetMap();
    private:
        std::unordered_map<EventType, void (*)()> events; // type, callback
    };

    inline GLFWwindow *GlobalWindow;
    inline EventSystem GlobalEventSystem;
    inline mold::render::objects::GameObjectsManager GlobalGameObjects;

    bool Init(uint width, uint height);
    void Run();
    void Destroy();
};
