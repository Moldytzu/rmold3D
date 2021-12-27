#include <rmold3D/mold.h>

uint VBO;
uint Shader;
uint TranslationMatrixLocation;

std::map<const char*, uint> ShaderUniforms;

#define Scale 0.5f

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth

    //creating translation matrix
    Matrix4 translation(
        1.0f, 0.0f, 0.0f, Scale * 2,
        0.0f, 1.0f, 0.0f, Scale,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f        
    );

    //telling opengl about the translation matrix
    glUniformMatrix4fv(TranslationMatrixLocation, 1, GL_TRUE, &translation.i[0][0]);

    //bind vbo
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,0,0); //define what type is our data

    //draw vbo
    glDrawArrays(GL_TRIANGLES,0,3);

    //disable vbo
    glDisableVertexAttribArray(0);

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

    //triangle
    Vector3 vertices[3];
    vertices[0] = Vector3(0,1,0); //up
    vertices[1] = Vector3(-1,-1,0); //down left
    vertices[2] = Vector3(1,-1,0); //down right

    //vbo for triangle
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //define shaders
    const char* vertexsrc =
    "#version 330 core\n" \
    "layout (location = 0) in vec3 Position;\n" \
    "uniform mat4 translationMatrix;\n" \
    "void main()\n" \
    "{\n" \
    " gl_Position = translationMatrix * vec4(Position, 1.0);\n" \
    "}\n" \
    "\n"; //gl position is the dot product of the translation matrix and the position itself

    const char* fragmentsrc =
    "#version 330 core\n" \
    "out vec4 FragColor;\n" \
    " \n" \
    "void main()\n" \
    "{\n" \
    " FragColor = vec4(1.0, 1.0, 0.0, 0.0);\n" \
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