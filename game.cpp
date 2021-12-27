#include <GL/glew.h>
#include <GL/glut.h>

class Vector3
{
public:
    float X;
    float Y;
    float Z;

    Vector3() {}
    Vector3(float x,float y,float z)
    {
        X = x;
        Y = y;
        Z = z;
    }
};

unsigned int VBO;

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

    glutMainLoop(); //run app

    return 0;
}