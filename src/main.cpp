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

#include "MarchingCubes.h"
#include <iostream>
#include <glm/glm.hpp>

using namespace glm;
using namespace std;

// Global variable for radius. Maybe we should find a better way to do this...
// You know, without a global variable.
int radius = 72;

void draw() {
   // Draw here.
}

void glut(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(100, 100);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Geometry Creator Demo");
   glutDisplayFunc(draw);

   glutMainLoop();
}

float implicitSphere(int x, int y, int z) {
   return x*x + y*y + z*z - radius*radius;
}

int main(int argc, char** argv) {
   MarchingCubes marchingCubes(vec3(128, 128, 128));

   marchingCubes.insideOutsideTest(implicitSphere, 0);

   glut(argc, argv);

   return 0;
}
