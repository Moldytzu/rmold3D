#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <cstring>

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