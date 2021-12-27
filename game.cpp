#include <rmold3D/mold.h>

uint VBO;

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth

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
    "\n" \
    "void main()\n" \
    "{\n" \
    " gl_Position = vec4(Position.x, Position.y, Position.z, 1.0);\n" \
    "}\n" \
    "\n";

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
    GLuint shader = glCreateProgram();
    attachShader(shader,vertexsrc,GL_VERTEX_SHADER);
    attachShader(shader,fragmentsrc,GL_FRAGMENT_SHADER);
    glLinkProgram(shader);

    int status;
    glGetProgramiv(shader, GL_LINK_STATUS, &status);

    if(!status)
    {
        printf("Error linking shader!\n");
        exit(1);
    }

    glUseProgram(shader);

    glutMainLoop(); //run app

    return 0;
}