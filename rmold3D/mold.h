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
#include <sstream>
#include <algorithm>
#include <csignal>
#include <vector>
#include <any>

#ifdef __WIN32__
#include "3rd-Party/mingw-std-threads-master/mingw.thread.h"
#else
#include <thread>
#endif

#define Vertex(X, Y, Z) X, Y, Z
#define TexCoord(X, Y) X, Y
#define Normal(X, Y, Z) X, Y, Z

typedef unsigned int uint;

#if __cplusplus < 201703L
#error rmold3D deppends on C++17 features to work. You either don't have them enabled or your compiler doesn't support them.
#endif

#define EndsWith(str, suffix) (str.size() >= suffix.size() && 0 == str.compare(str.size() - suffix.size(), suffix.size(), suffix))
#define StartsWith(str, prefix) (str.size() >= prefix.size() && 0 == str.compare(0, prefix.size(), prefix))

#define GetAny(any, type) ((type)std::any_cast<type>(any))

namespace mold
{
    enum EventType
    {
        Redraw,
        Tick,
        Resize,
        Exit,
        Mouse,
        LightTick,
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

                std::unordered_map<std::string, mold::render::image::Texture> GetDB(); // get database of textures

                uint32_t Size;
                uint32_t Width;
                uint32_t Height;
                uint8_t *PixelData;

            private:
                void CreateIndex();
                uint TextureIndex = 0;
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
            uint *Shaders = nullptr;
            uint ShadersIndex = 0;
        };

        inline std::string VertexShaderSource = R"V0G0N(
#version 330 core

layout (location = 0) in vec3 vertexPosition;
layout (location = 1) in vec2 textureCoordornate;
                                                
out vec2 textureCoord;
out float visibility;
out vec3 vertexPos;
                                                
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float fogDensity;
uniform bool fogEnabled;

void calculateFog()
{
    float distance = length(gl_Position.xyz);
    visibility = exp(-pow(distance*fogDensity,1.5));
    visibility = clamp(visibility,0.0,1.0);
}

void main()
{
    gl_Position = projection * view * model * vec4(vertexPosition, 1.0);
    textureCoord = textureCoordornate;
    vertexPos = (model * vec4(vertexPosition, 1.0)).xyz;

    if(fogEnabled == true)
    {
        calculateFog();
    }
}
)V0G0N";

        inline std::string FragmentShaderSource = R"V0G0N(
#version 330 core

out vec4 FragColor;

in vec2 textureCoord;
in float visibility;
in vec3 vertexPos;

uniform vec4 fcolour;
uniform vec4 fogColour;
uniform bool fogEnabled;

uniform sampler2D mainTexture;

uniform bool lightingEnabled;
uniform float lightingAmbient;

uniform float gamma;
uniform bool gammaCorectionEnabled;

struct Light  {
   vec3 Position;
   float Power; 
   vec3 Colour;
};

#define LIGHTS 128
uniform Light lights[LIGHTS];

vec3 calculateLight(vec4 inputColour, Light light)
{
   vec3 normal = normalize(vec3(mat4(1.0) * vec4(1.0,1.0,1.0, 0.0)));
   vec3 lightDir = abs(light.Position - vertexPos);
   float distance = length(lightDir);
   distance = distance * distance;
   float lambertian = max(dot(lightDir, normal), 0.0);
   float specular = 0.0;
   if (lambertian > 0.0) {
       vec3 viewDir = normalize(-vertexPos);
        vec3 halfDir = normalize(lightDir + viewDir);
        float specAngle = max(dot(halfDir, normal), 0.0);
        specular = pow(specAngle, 256);
    }
   return inputColour.xyz * lambertian * light.Colour * light.Power / distance + light.Colour * specular * light.Colour * light.Power / distance;
}

void main()
{
   FragColor = texture(mainTexture, textureCoord) * vec4(vec3(lightingAmbient),1.0);

   if(lightingEnabled == true)
   {
      for(int i = 0;i <LIGHTS;i++)
      {
          if(lights[i].Power != 0)
            FragColor = vec4(calculateLight(FragColor, lights[i]),1.0);
      }
   }

   if(fogEnabled == true)
   {
      FragColor = mix(fogColour, FragColor, visibility);
   }

   if(gammaCorectionEnabled == true)
   {
      FragColor.rgb = pow(FragColor.rgb, vec3(1.0/2.2));
   }

   FragColor.rgb = FragColor.rgb * gamma;
}
)V0G0N";

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
            Skybox(Colour c);

            void Bind();
            void Deallocate();

        private:
            VABO Vabo;
            image::Texture upT, sideT, downT;
        };

        namespace camera
        {
            inline glm::vec3 Position = glm::vec3(0.0f, 0.0f, 0.0f);
            inline glm::vec3 Front = glm::vec3(0.0f, 0.0f, -1.0f);
            inline glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
            inline float Yaw = -90.0f; //-90.0f left, 0 front, 90.0f right
            inline float Pitch = 0.0f; // 90.0f up, 0 front, -90.0f down

            inline glm::mat4 View;
            inline glm::mat4 Projection;

            void Translate(glm::vec3 axis, float value);
            void Rotate(glm::vec3 axis, float value);

            bool InView(glm::vec3 position);

            void Handle();
        };

        namespace objects
        {
            class GameObjectBase;

            class Component
            {
            public:
                virtual void Tick();
                virtual void Start();
                virtual void Handle(mold::EventType event);

                void CallFunc(std::string publicName);
                void Expose(void (*fn)(mold::render::objects::Component *), std::string publicName);

                bool Enabled = true;
                GameObjectBase *Parent = nullptr;

                std::unordered_map<std::string, std::any> Public;
            };

            class CubeRenderer : public Component
            {
            public:
                mold::render::VABO Vabo;
                void Start();
                void Tick();
            };

            class PlaneRenderer : public Component
            {
            public:
                mold::render::VABO Vabo;
                void Start();
                void Tick();
            };

            class PointLightRenderer : public Component
            {
            public:
                void Start();
                void Handle(mold::EventType event);
            };

            class GameObjectBase
            {
            public:
                ~GameObjectBase();

                GameObjectBase();

                virtual GameObjectBase *Translate(glm::vec3 offset); // translate position
                virtual GameObjectBase *Move(glm::vec3 position);    // set position
                GameObjectBase *Scale(glm::vec3 scaleFactor);        // set scale factor
                GameObjectBase *Rotate(glm::vec3 axis, float angle); // set rotation

                GameObjectBase *Translate(float x, float y, float z);           // translate position
                GameObjectBase *Move(float x, float y, float z);                // set position
                GameObjectBase *Scale(float x, float y, float z);               // set scale factor
                GameObjectBase *Rotate(float x, float y, float z, float angle); // set rotation

                void Bind();             // bind everything
                glm::vec3 GetPosition(); // get position

                GameObjectBase *AttachComponent(std::string name, Component *component);      // attach component
                GameObjectBase *DettachComponent(std::string name);                           // dettach component
                bool ExistsComponent(std::string name);                                       // check if component exists
                void TickComponents();                                                        // tick every component
                void HandleComponents(mold::EventType event);                                 // forward events to every component
                std::unordered_map<std::string, mold::render::objects::Component *> Get();    // get components attached
                mold::render::objects::Component *Get(std::string name);                      // get component with the name
                std::unordered_map<std::string, mold::render::objects::Component *> GetRaw(); // get components attached to everything

                virtual std::string Type();

                glm::mat4 PositionMatrix = glm::mat4(1.0f);

                bool Enabled = false;

                float Opacity = 1.0f;

                std::string Name;
            };

            class Prefab : public GameObjectBase
            {
            public:
                virtual void Start();
            };

            class Cube : public Prefab
            {
            public:
                void Start();
            };

            class Plane : public Prefab
            {
            public:
                void Start();
            };

            class PointLight : public Prefab
            {
            public:
                void Start();
            };

            class Light : public GameObjectBase
            {
            public:
                Light();
                Light(glm::vec3 pos);
                Light(glm::vec3 pos, glm::vec3 col);
                Light(glm::vec3 pos, glm::vec3 col, float power);

                GameObjectBase *Translate(glm::vec3 offset); // translate position
                GameObjectBase *Move(glm::vec3 position);    // set position

                void Draw();
                std::string Type();

                glm::vec3 Position, Colour;
                float Power = 0;
            };

            class Empty : public GameObjectBase
            {
            public:
                Empty();
            };

            class GameObjectsManager
            {
            public:
                GameObjectBase *Instantiate(GameObjectBase *object);
                GameObjectBase *Instantiate(GameObjectBase *object, std::string name);
                GameObjectBase *Instantiate(Prefab *object);
                GameObjectBase *Instantiate(Prefab *object, std::string name);
                void Remove(std::string name);
                bool Exists(std::string name);
                GameObjectBase *Get(std::string name);
                std::unordered_map<std::string, GameObjectBase *> Get();

            private:
                void Add(std::string name, GameObjectBase *object);
                std::unordered_map<std::string, GameObjectBase *> GameObjects;
            };
        };

        void DrawTriangles(uint count);
        void HandleFog();
        void HandleErrors();
        void Render();
        void Init(uint width, uint height);

        void OnResize(GLFWwindow *window, int width, int height);
        void OnError(int id, const char *description);

        inline uint LightIdx = 0;
    };

    namespace memory
    {
        inline size_t Allocated = 0;
        inline bool Track = false;

        void Free(); // free every piece of allocated memory
    };

    namespace profiler
    {
        inline size_t glVertices = 0;
    };

    namespace math
    {
        float Vfov(float Hfov, float aspectRatio);
    };

    namespace settings
    {
        inline float FOV = 90.0f;                  // field of view
        inline float WindowWidth;                  // window width
        inline float WindowHeight;                 // window height
        inline float MouseSensibility = 1.0f;      // mouse sensibility
        inline bool FogEnabled = true;             // enable fog
        inline float FogDensity = 0.09f;           // fog density
        inline glm::vec4 FogColour = glm::vec4(1); // fog colour
        inline bool FogAutoDensity = true;         // dynamically set fog density depending on the skybox distance
        inline float LightingEnabled = true;       // lighting
        inline float LightingAmbient = 0.1f;       // ambient lighting
        inline float SkyboxDistance = 25;          // distance to the skybox
        inline bool MSAAEnabled = true;            // enable msaa
        inline bool VSYNC = true;                  // vertical sync
        inline bool FullScreen = false;            // full screen window
        inline bool TransparencyEnabled = true;    // enable transparency
        inline bool GammaCorrection = true;        // enable gamma correction
        inline float Gamma = 1.0f;                 // gamma
        inline bool Debug = false;                 // debug engine
        inline bool DebugRenderer = false;         // debug renderer
        inline float AspectRatio = 1.77777777777f; // aspect ratio
        inline float FrameLimit = 0;               // max fps

        void Update();                           // update settings
        void LoadFromFile(std::string filename); // load settings from file
        void SaveToFile(std::string filename);   // save settings to file
    };

    namespace time
    {
        inline float DeltaTime = 0;
        inline float LastFrame = 0;
        inline int FPS = 0;
    };

    enum CursorLockingMode
    {
        Locked,
        Hidden,
        Normal,
        Centred,
    };

    namespace input
    {
        class InputManager
        {
        public:
            InputManager();
            InputManager *Map(std::string mapping, int key);
            InputManager *UnMap(std::string mapping);
            float Get(std::string mapping);

        private:
            std::unordered_map<std::string, int> Mappings;
        };

        void HandleCursor();
        void OnScroll(GLFWwindow *window, double xoffset, double yoffset);
        void OnMouse(GLFWwindow *window, double xpos, double ypos);

        inline CursorLockingMode GlobalCursorLockMode = CursorLockingMode::Normal;
    };

    namespace log
    {
        void Info(std::string str);
        void Warn(std::string str);
        void Error(std::string str);
        void Fatal(std::string str);
        void Debug(std::string str);
    };

    namespace Events
    {
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

    class ThreadManager
    {
    public:
        void Add(std::thread *t);
        void Wait();
        void Reset();
    };

    inline GLFWwindow *GlobalWindow = nullptr;
    inline Application *GlobalApplication = nullptr;
    inline ThreadManager GlobalThreads;
    inline input::InputManager GlobalInputManager;
    inline render::Shader GlobalShader;
    inline render::Skybox GlobalSkybox;
    inline render::objects::GameObjectsManager GlobalGameObjects;

    void Destroy();
    void HandleSignal(int signum);
    Application *BuildApplication();

    void Init(uint width, uint height);
    void Run();

};
