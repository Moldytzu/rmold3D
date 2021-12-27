#include <GL/glew.h>
#include <GL/glut.h>
#include <stdio.h>
#include <cstring>
#include <map>
#include <time.h>

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

class Matrix4 //4x4 matrix
{
public:
    float i[4][4];

    Matrix4() {}
    Matrix4(float v00, float v01, float v02, float v03, //vertices
             float v10, float v11, float v12, float v13,
             float v20, float v21, float v22, float v23,
             float v30, float v31, float v32, float v33)
    {
        i[0][0] = v00; i[0][1] = v01; i[0][2] = v02; i[0][3] = v03;
        i[1][0] = v10; i[1][1] = v11; i[1][2] = v12; i[1][3] = v13;
        i[2][0] = v20; i[2][1] = v21; i[2][2] = v22; i[2][3] = v23;
        i[3][0] = v30; i[3][1] = v31; i[3][2] = v32; i[3][3] = v33;
    }
};

static float RandomFloat(float a, float b) {
    float random = ((float) rand()) / (float) RAND_MAX;
    float diff = b - a;
    float r = random * diff;
    return a + r;
}

class Vertex
{
public:
    Vector3 position;
    Vector3 color;

    Vertex() {}
    Vertex(float x,float y,float z)
    {
        position = Vector3(x,y,z);
        color = Vector3(RandomFloat(0,1),RandomFloat(0,1),RandomFloat(0,1));
    }
};