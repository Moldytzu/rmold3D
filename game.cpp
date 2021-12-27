#include <GL/glew.h>
#include <GL/glut.h>

void render()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //clear color and depth

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

    //init glew
    glewInit();

    glClearColor(0,0,0,0); // black

    glutDisplayFunc(render); //on render

    glutMainLoop(); //run app

    return 0;
}