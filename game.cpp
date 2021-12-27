#include <rmold3D/mold.h>

uint VBO;
uint IBO;
uint Shader;
uint TranslationMatrixLocation;

std::map<const char*, uint> ShaderUniforms;

#define Scale 0.5f

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth

    //bind vbo & ibo
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    //position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,6 * sizeof(float),0); //define what type is our data

    //color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,6 * sizeof(float), (void*)(3*sizeof(float))); //define what type is our data

    //draw vbo
    glDrawElements(GL_TRIANGLES,3,GL_UNSIGNED_INT,0);

    //disable vbo
    glDisableVertexAttribArray(0);
    glDisableVertexAttribArray(1);

    glutPostRedisplay(); //call again redraw
    glutSwapBuffers(); //do double buffering
}

void attachShader(uint program, const char* src, uint type)
{
    uint obj = glCreateShader(type); //create shader with the type specified

    int len = strlen(src);
    glShaderSource(obj,1,&src,&len); //define source
    glCompileShader(obj); //compile shader

    int status;
    glGetShaderiv(obj, GL_COMPILE_STATUS, &status);

    if(!status)
    {
        printf("Error compiling shader!\n");
        exit(1);
    }

    glAttachShader(program, obj);
}

int main(int argc, char** argv)
{
    //init glut
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE); //rgba, double buffering, depth
    glutInitWindowSize(800,600); //800x600
    glutCreateWindow("rmold3D"); //create window
    glutDisplayFunc(render); //on render

    //init glew
    glewInit();
    glClearColor(0,0,0,0); // black
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    //init rand
    srand(time(NULL));

    //triangle
    Vertex vertices[3];
    vertices[0] = Vertex(0,1,0); //up
    vertices[1] = Vertex(-1,-1,0); //down left
    vertices[2] = Vertex(1,-1,0); //down right

    //vbo for triangle
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //ibo for triangle
    uint Indices[] = {
        0,1,2,
        3,4,5,
        6,7,8,
    };
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);

    //define shaders
    const char* vertexsrc =
    "#version 330 core\n" \
    "layout (location = 0) in vec3 Position;\n" \
    "layout (location = 1) in vec3 inColor;\n" \
    "out vec4 Color;\n" \
    "void main()\n" \
    "{\n" \
    " gl_Position = vec4(Position, 1.0);\n" \
    " Color = vec4(inColor, 1.0);\n" \
    "}\n" \
    "\n"; //gl position is the dot product of the translation matrix and the position itself

    const char* fragmentsrc =
    "#version 330 core\n" \
    "out vec4 FragColor;\n" \
    "in vec4 Color;\n" \
    " \n" \
    "void main()\n" \
    "{\n" \
    " FragColor = Color;\n" \
    "}\n" \
    "\n";

    //compile & link shader
    Shader = glCreateProgram();
    attachShader(Shader,vertexsrc,GL_VERTEX_SHADER);
    attachShader(Shader,fragmentsrc,GL_FRAGMENT_SHADER);
    glLinkProgram(Shader);

    int status;
    glGetProgramiv(Shader, GL_LINK_STATUS, &status);

    if(!status)
    {
        printf("Error linking shader!\n");
        exit(1);
    }

    glUseProgram(Shader);

    glutMainLoop(); //run app

    return 0;
}