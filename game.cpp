#include <rmold3D/mold.h>

#define WINDOW_HEIGHT (float)600
#define WINDOW_WIDTH (float)800

#define SetUniformf(name, value) glUniform1f(glGetUniformLocation(shaderProgram, name), value);
#define SetUniformi(name, value) glUniform1i(glGetUniformLocation(shaderProgram, name), value);

unsigned int VBO;
unsigned int VAO;
unsigned int EBO;
unsigned int shaderProgram;
unsigned int texture;

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

Texture LoadRGBBitmap(const char *filename)
{
    FILE *file;
    uint32_t size;
    uint16_t bitPlanes;
    uint16_t bpp;

    file = fopen(filename, "rb");

    if ((uint64_t)file == 0)
    {
        char buffer[1024];
        sprintf(buffer, "Couldn't find the bitmap image %s!", filename);

        puts(buffer);

        return {};
    }

    BitmapImageHeader *header = (BitmapImageHeader *)malloc(sizeof(BitmapImageHeader));
    if (fread(header, sizeof(BitmapImageHeader), 1, file) != 1)
        return {};

    if (header->BPP != 24)
        return {};

    Texture texture;

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

//glfw callbacks
void onResize(GLFWwindow *window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void onKey(GLFWwindow *window, int key, int scancode, int action, int mods)
{
}

//draw on the screen
void onDraw()
{
    glClearColor(0, 0, 0, 0);     //black
    glClear(GL_COLOR_BUFFER_BIT); //clear screen

    // create transformations
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    projection = glm::perspective(glm::radians(45.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 100.0f);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"), 1, GL_FALSE, &model[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, &view[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, &projection[0][0]);

    glBindTexture(GL_TEXTURE_2D, texture);
    glBindVertexArray(VAO); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//clean up the mess
void destroy()
{
    glfwTerminate();
    exit(-1);
}

//entry point
int main()
{
    glfwInit(); // initialize glfw
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // GL 3.3 Core

    GLFWwindow *window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rewritten mold 3D", NULL, NULL); // create window
    if (window == NULL)                                                                                  // exit if the window couldn't be created
        destroy();

    glfwMakeContextCurrent(window); // create opengl context

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) // load glad
        destroy();

    // set up callbacks
    glfwSetFramebufferSizeCallback(window, onResize);

    // set up gl
    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glm::ortho(0.0f, WINDOW_WIDTH, 0.0f, WINDOW_HEIGHT, 0.1f, 100.0f);

    //compile shaders
    const char *vertexShaderSource = "#version 330 core\n"
                                     "layout (location = 0) in vec3 vertexPosition;\n"
                                     "layout (location = 1) in vec3 vertexColor;\n"
                                     "layout (location = 2) in vec2 textureCoordornate;\n"
                                     "out vec3 color;\n"
                                     "out vec2 textureCoord;\n"
                                     "uniform mat4 model;\n"
                                     "uniform mat4 view;\n"
                                     "uniform mat4 projection;\n"
                                     "void main()\n"
                                     "{\n"
                                     "   gl_Position = projection * view * model * vec4(vertexPosition, 1.0);\n"
                                     "   color = vertexColor;\n"
                                     "   textureCoord = textureCoordornate;\n"
                                     "}\n";

    const char *fragmentShaderSource = "#version 330 core\n"
                                       "out vec4 FragColor;\n"
                                       "in vec3 color;\n"
                                       "in vec2 textureCoord;\n"
                                       "uniform sampler2D mainTexture;\n"
                                       "void main()\n"
                                       "{\n"
                                       "   FragColor = texture(mainTexture, textureCoord);\n"
                                       "}\n";

    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER); //create and compile vertex shader
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    //check for errors
    int success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success)
        destroy();

    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER); //create and compile fragment shader
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    //check for errors
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
        destroy();

    shaderProgram = glCreateProgram(); //link the shaders together to form a program
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    //check for errors
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
        destroy();

    //use shader
    glUseProgram(shaderProgram);

    //clean up the objects we don't need so we use less vram
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    //triangles
    float vertices[] = {
        // positions     /      colors      / texture coords
        0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,   // top right
        0.5f, -0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
        -0.5f, 0.5f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f   // top left
    };

    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    //set up a vbo, a vao & an ebo
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);                                        //tell opengl we want to use this vbo
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //set data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)0);                   //position attribute
    glEnableVertexAttribArray(0);                                                                    //enable position
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(3 * sizeof(float))); //color attribute
    glEnableVertexAttribArray(1);                                                                    //enable color
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void *)(6 * sizeof(float))); //texture attribute
    glEnableVertexAttribArray(2);                                                                    //enable texture

    //load texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    Texture t = LoadRGBBitmap("texture.bmp");
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, t.Width, t.Height, 0, GL_RGB, GL_UNSIGNED_BYTE, t.PixelData);
    glGenerateMipmap(GL_TEXTURE_2D);

    //main loop
    while (!glfwWindowShouldClose(window))
    {
        //draw stuff
        onDraw();

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    destroy();
    return 0;
}