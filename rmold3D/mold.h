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
            void Set(std::string location, float value);
            void Set(std::string location, bool value);

        private:
            uint Program = 0xdeadbeef;
            uint *Shaders;
            uint ShadersIndex = 0;
        };

        inline std::string VertexShaderSource = "#version 330 core\n"
                                                "layout (location = 0) in vec3 vertexPosition;\n"
                                                "layout (location = 1) in vec2 textureCoordornate;\n"
                                                "out vec2 textureCoord;\n"
                                                "out float visibility;\n"
                                                "uniform mat4 model;\n"
                                                "uniform mat4 view;\n"
                                                "uniform mat4 projection;\n"
                                                "uniform float fogDensity;\n"
                                                "void main()\n"
                                                "{\n"
                                                "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
                                                "   float dis = length(gl_Position.xyz);\n"
                                                "   visibility = exp(-pow(dis*fogDensity,1.5));\n"
                                                "   visibility = clamp(visibility,0.0,1.0);\n"
                                                "   textureCoord = textureCoordornate;\n"
                                                "}\n";

        inline std::string FragmentShaderSource = "#version 330 core\n"
                                                  "out vec4 FragColor;\n"
                                                  "in vec2 textureCoord;\n"
                                                  "in float visibility;\n"
                                                  "uniform vec4 fcolour;\n"
                                                  "uniform sampler2D mainTexture;\n"
                                                  "uniform bool fogEnabled;\n"
                                                  "uniform vec4 fogColour;\n"
                                                  "void main()\n"
                                                  "{\n"
                                                  "   FragColor = texture(mainTexture, textureCoord) * fcolour;\n"
                                                  "   if(fogEnabled == true)\n"
                                                  "      {FragColor = mix(fogColour, FragColor, visibility);}\n"
                                                  "}\n";

        namespace lighting
        {
            inline glm::vec3 SunPosition = glm::vec3(1.5f);
        };

        namespace fog
        {
            inline bool Enabled = true;
            inline float Density = 0.25f;
            inline glm::vec4 Colour = glm::vec4(1);
        };

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

            float Scale = 25.0f;

        private:
            VABO Vabo;
            image::Texture upT, sideT, downT;
        };

        namespace camera
        {
            inline glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
            inline glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
            inline glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            inline float Yaw = -90.0f; //-90.0f left, 0 front, 90.0f right
            inline float Pitch = 0.0f; // 90.0f up, 0 front, -90.0f down

            inline glm::mat4 View;
            inline glm::mat4 Projection;

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
