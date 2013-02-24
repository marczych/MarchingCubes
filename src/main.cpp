#ifdef __APPLE__
#include "GLUT/glut.h"
#include <OPENGL/gl.h>
#endif

#ifdef __unix__
#include <GL/glut.h>
#endif

#ifdef _WIN32
#include <GL/glew.h>
#include <GL/glut.h>

#pragma comment(lib, "glew32.lib")
#pragma comment(lib, "OpenGL32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "freeglut.lib")
#endif

#include "MStackHelp.h"
#include "MarchingCubes.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

RenderingHelper ModelTrans;

// Global variable for radius. Maybe we should find a better way to do this...
// You know, without a global variable.
int radius = 72;

void draw() {
   // Draw here.
}

void glut(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(800, 600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Geometry Creator Demo");
   glutDisplayFunc(draw);

   glutMainLoop();
}

void openGLInitialize() {
    glClearColor(0.8f, 0.8f, 1.0f, 1.0f);

    glClearDepth(1.0f);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_DEPTH_TEST);

    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    ModelTrans.useModelViewMatrix();
    ModelTrans.loadIdentity();
}

float implicitSphere(int x, int y, int z) {
   return x*x + y*y + z*z - radius*radius;
}

int main(int argc, char** argv) {
   MarchingCubes marchingCubes(vec3(128, 128, 128));

   marchingCubes.insideOutsideTest(implicitSphere, 0);
   marchingCubes.generateSurfaces();

   glut(argc, argv);

   return 0;
}
