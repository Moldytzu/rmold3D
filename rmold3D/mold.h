#include "3rd-Party/glad/include/glad/glad.h"
#include "3rd-Party/glm/glm/glm.hpp"
#include "3rd-Party/glm/glm/gtc/matrix_transform.hpp"
#include "3rd-Party/glm/glm/gtc/type_ptr.hpp"
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
#define Normal(X, Y, Z) X, Y, Z

typedef unsigned int uint;

#if __cplusplus < 201703L
#error rmold3D deppends on C++17 features to work. You either don't have them enabled or your compiler doesn't support them.
#endif

#define EndsWith(str, suffix) (str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix))
#define StartsWith(str, prefix) (str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix))

namespace mold
{
    enum EventType
    {
        Redraw,
        Tick,
        Resize,
        Exit,
        Mouse,
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

        class Shader
        {
        public:
            Shader();
            void Recompile();
            void Deallocate();
            void Bind();
            void AttachSource(std::string source, uint type);

            void Set(std::string location, glm::mat4 matrix);
            void Set(std::string location, glm::vec4 vector);
            void Set(std::string location, glm::vec3 vector);
            void Set(std::string location, int value);

        private:
            uint Program = 0xdeadbeef;
            uint *Shaders;
            uint ShadersIndex = 0;
        };

        inline std::string VertexShaderSource = "#version 330 core\n"
                                                "layout (location = 0) in vec3 vertexPosition;\n"
                                                "layout (location = 1) in vec2 textureCoordornate;\n"
                                                "layout (location = 2) in vec3 normals;\n"
                                                "out vec2 textureCoord;\n"
                                                "out vec4 icolour;\n"
                                                "out vec3 norms;\n"
                                                "uniform mat4 model;\n"
                                                "uniform mat4 view;\n"
                                                "uniform mat4 projection;\n"
                                                "uniform vec4 fcolour;\n"
                                                "void main()\n"
                                                "{\n"
                                                "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
                                                "   textureCoord = textureCoordornate;\n"
                                                "   icolour = fcolour;\n"
                                                "   norms = mat3(transpose(inverse(model))) * normals;\n"
                                                "}\n";

        inline std::string FragmentShaderSource = "#version 330 core\n"
                                                  "out vec4 FragColor;\n"
                                                  "in vec2 textureCoord;\n"
                                                  "in vec3 norms;\n"
                                                  "in vec4 icolour;\n"
                                                  "uniform sampler2D mainTexture;\n"
                                                  "uniform vec3 sunPos;\n"
                                                  "uniform vec3 objPos;\n"
                                                  "void main()\n"
                                                  "{\n"
                                                  "   float ambient = 0.2f;"
                                                  "   vec3 normal = normalize(norms);\n"
                                                  "   vec3 direction = normalize(sunPos-objPos);\n"
                                                  "   float diffuse = max(dot(normal,direction),0.0f);\n"
                                                  "   vec3 reflect = reflect(-direction, normal);\n"
                                                  "   float spec = pow(max(dot(direction, reflect), 0.0), 8);\n"
                                                  "   float dist = length(sunPos-objPos);\n"
                                                  "   float attenuation = 1.0f / (dist * dist);"
                                                  "   float lighting = diffuse + ambient + spec;"
                                                  "   FragColor = texture(mainTexture, textureCoord) * icolour;\n"
                                                  "}\n";

        namespace lighting
        {
            inline glm::vec3 SunPosition = glm::vec3(1.5f);
        };

        inline std::string SkyboxFragmentSource = "#version 330 core\n"
                                            "out vec4 FragColor;\n"
                                            "in vec3 textureCoord;\n"
                                            "uniform samplerCube skybox;\n"
                                            "void main()\n"
                                            "{\n"
                                            "   FragColor = texture(skybox, textureCoord);\n"
                                            "}\n";

        inline std::string SkyboxVertexSource = "#version 330 core\n"
                                          "layout (location = 0) in vec3 aPos;\n"
                                          "out vec3 TexCoords;\n"
                                          "uniform mat4 projection;\n"
                                          "uniform mat4 view;\n"
                                          "void main()\n"
                                          "{\n"
                                          "   TexCoords = aPos;\n"
                                          "   gl_Position = projection * view * vec4(aPos, 1.0);\n"
                                          "}\n";

        class VABO
        {
        public:
            VABO();
            VABO(float *vertices, size_t len);

            void Bind();
            void Deallocate();

        private:
            uint VAO = 0, VBO = 0;
        };    

        class Skybox
        {
        public:
            Skybox();
            Skybox(std::string up, std::string side, std::string down);

            void Bind();
            void Deallocate();

            bool Enabled = false;
            Shader SkyShader;
        
        private:
            uint TextureID;
            uint VAO = 0, VBO = 0;
        };

        namespace camera
        {
            inline glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            inline glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
            inline glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            inline float Yaw = -90.0f; //-90.0f left, 0 front, 90.0f right
            inline float Pitch = 0.0f; // 90.0f up, 0 front, -90.0f down

            void Translate(glm::vec3 axis, float value);
            void Rotate(glm::vec3 axis, float value);

            bool InView(glm::vec3 position);
        };

        namespace objects
        {
            class GameObject;

            class Component
            {
            public:
                virtual void Tick();
                virtual void Start();
                virtual void Handle(mold::EventType event);

                bool Enabled = true;
                GameObject *Parent = nullptr;
            };

            class GameObject
            {
            public:
                ~GameObject();

                GameObject();
                GameObject(mold::render::image::Texture texture);

                GameObject *Translate(glm::vec3 offset);         // translate position
                GameObject *Move(glm::vec3 position);            // set position
                GameObject *Scale(glm::vec3 scaleFactor);        // set scale factor
                GameObject *Rotate(glm::vec3 axis, float angle); // set rotation

                GameObject *Translate(float x, float y, float z);           // translate position
                GameObject *Move(float x, float y, float z);                // set position
                GameObject *Scale(float x, float y, float z);               // set scale factor
                GameObject *Rotate(float x, float y, float z, float angle); // set rotation

                void ReplaceTexture(mold::render::image::Texture newTexture); // replace the texture with a new one
                void Bind();                                                  // bind everything
                glm::vec3 GetPosition();                                      // get position

                void AttachComponent(std::string name, Component *component);                        // attach component
                void DettachComponent(std::string name);                                             // dettach component
                bool ExistsComponent(std::string name);                                              // check if component exists
                void TickComponents();                                                               // tick every component
                void HandleComponents(mold::EventType event);                                        // forward events to every component
                std::unordered_map<std::string, mold::render::objects::Component *> GetComponents(); // get components attached

                virtual void Draw();
                virtual std::string Type();

                glm::mat4 PositionMatrix = glm::mat4(1.0f);

                bool Enabled = false;

                float Opacity = 1.0f;

                std::string Name;

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

            class Plane : public GameObject
            {
            public:
                Plane();
                Plane(mold::render::image::Texture texture);

                void Draw();
                std::string Type();
            };

            class Empty : public GameObject
            {
            public:
                Empty();
            };

            class GameObjectsManager
            {
            public:
                GameObject *Instantiate(GameObject *object);
                GameObject *Instantiate(GameObject *object, std::string name);
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
        inline float ViewDistanceDivisor = 10.0f;
        inline float WindowWidth;
        inline float WindowHeight;
        inline float MouseSensibility = 5.0f;
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
        void Fatal(std::string str);
    };

    class EventSystem
    {
    public:
        void CallEvent(EventType type);
    };

    class Application
    {
    public:
        Application();
        Application(uint width, uint height);
        virtual void Tick();
        virtual void OnDraw();
        virtual void OnResize();
        virtual void OnMouseInput();
        virtual std::string Name();
    };

    inline GLFWwindow *GlobalWindow;
    inline EventSystem GlobalEventSystem;
    inline Application *GlobalApplication;
    inline render::Shader GlobalShader;
    inline render::Skybox GlobalSkybox;
    inline render::objects::GameObjectsManager GlobalGameObjects;

    void Destroy();

    Application *BuildApplication();

    void Init(uint width, uint height);
    void Run();

};
