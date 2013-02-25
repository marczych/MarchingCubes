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

#include "GLSL_helper.h"
#include "MStackHelp.h"
#include "MarchingCubes.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace glm;
using namespace std;

vec3 volumeSize(128);
MarchingCubes marchingCubes(volumeSize);

int shaderProgram;

//Handles to the shader data
GLint h_aPosition;
GLint h_uNormal;
GLint h_uModelMatrix;
GLint h_uViewMatrix;
GLint h_uProjMatrix;

GLuint CubeBuffObj, CubeIdxBuffObj;

/* globals to control positioning and window size */
float g_width, g_height;
float g_viewangle = 0;

float modelAngle = 0.0f;

RenderingHelper ModelTrans;

vector<vec3> cubeNormals;

// Global variable for radius. Maybe we should find a better way to do this...
// You know, without a global variable.
int radius = sqrt(dot(volumeSize, volumeSize)) * 0.25f;

/* projection matrix - do not change - sets matrix in shader */
void SetProjectionMatrix() {
  glm::mat4 Projection = glm::perspective(80.0f, (float)g_width/g_height, 0.1f, 500.f);
  safe_glUniformMatrix4fv(h_uProjMatrix, glm::value_ptr(Projection));
}

/* camera controls - do not change - sets matrix in shader */
void SetView() {
  glm::mat4 Trans = glm::translate( glm::mat4(1.0f),
   glm::vec3(-volumeSize.x * 0.5f, -volumeSize.y * 0.5f, -volumeSize.z * 1.25f));
  glm::mat4 RotateX = glm::rotate( Trans, g_viewangle, glm::vec3(0.0f, 1, 0));
  safe_glUniformMatrix4fv(h_uViewMatrix, glm::value_ptr(RotateX));
}

/* model transforms - do not change - sets matrix in shader */
void SetModel() {
  safe_glUniformMatrix4fv(h_uModelMatrix, glm::value_ptr(ModelTrans.modelViewMatrix));
}

int getTriangleIndex(int edge1, int edge2, int edge3) {
   return ((edge1 << 8) | (edge2 << 4) | edge3) * 3;
}

vec3 floatToVec3(float* start) {
   return vec3(start[0], start[1], start[2]);
}

void InitCubeTriangles() {
   float cubeEdges[] = {
      0.0f, 0.5f, 0.5f,
      0.5f, 0.5f, 0.0f,
      0.0f, 0.5f, -0.5f,
      -0.5f, 0.5f, 0.0f,

      0.0f, -0.5f, 0.5f,
      0.5f, -0.5f, 0.0f,
      0.0f, -0.5f, -0.5f,
      -0.5f, -0.5f, 0.0f,

      -0.5f, 0.0f, 0.5f,
      0.5f, 0.0f, 0.5f,
      0.5f, 0.0f, -0.5f,
      -0.5f, 0.0f, -0.5f
   };

   short cubeIndex[((1 << 13) - 1) * 3];
   cubeNormals.resize(((1 << 13) - 1) * 3);
   memset(cubeIndex, -1, sizeof(cubeIndex));

   for (int i = 0; i < 256; i++) {
      const int* triangleRow = MarchingCubes::triTable[i];

      for (int k = 0; triangleRow[k] != -1; k += 3) {
         int index = getTriangleIndex(triangleRow[k], triangleRow[k + 1],
          triangleRow[k + 2]);

         if (cubeIndex[index] == -1) {
            cubeIndex[index] = triangleRow[k];
            cubeIndex[index + 1] = triangleRow[k + 1];
            cubeIndex[index + 2] = triangleRow[k + 2];

            vec3 edge1 = floatToVec3(cubeEdges + triangleRow[k + 1]) -
                         floatToVec3(cubeEdges + triangleRow[k]);
            vec3 edge2 = floatToVec3(cubeEdges + triangleRow[k + 2]) -
                         floatToVec3(cubeEdges + triangleRow[k + 1]);
            vec3 normal = normalize(cross(edge1, edge2));

            cubeNormals[index / 3] = normal;
         }
      }
   }

   glGenBuffers(1, &CubeBuffObj);
   glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
   glBufferData(GL_ARRAY_BUFFER, sizeof(cubeEdges), cubeEdges, GL_STATIC_DRAW);

   glGenBuffers(1, &CubeIdxBuffObj);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeIdxBuffObj);
   glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cubeIndex), cubeIndex, GL_STATIC_DRAW);
}

void draw() {
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   glUseProgram(shaderProgram);

   SetProjectionMatrix();
   SetView();

   safe_glEnableVertexAttribArray(h_aPosition);
   glBindBuffer(GL_ARRAY_BUFFER, CubeBuffObj);
   safe_glVertexAttribPointer(h_aPosition, 3, GL_FLOAT, GL_FALSE, 0, 0);
   glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, CubeIdxBuffObj);

   ModelTrans.pushMatrix(); {
      ModelTrans.translate(volumeSize/2.0f);
      ModelTrans.rotate(modelAngle, vec3(0.0, -1.0f, 0.0));
      ModelTrans.translate(volumeSize/-2.0f);

      const vector<unsigned char>& cubes = marchingCubes.getCubes();

      for (unsigned int i = 0; i < cubes.size(); i++) {
         ModelTrans.pushMatrix();

         ModelTrans.translate((vec3)marchingCubes.coordinate(i));

         SetModel();

         const int* triangleRow = MarchingCubes::triTable[cubes[i]];
         for (int k = 0; triangleRow[k] != -1; k += 3) {
            int index = getTriangleIndex(triangleRow[k], triangleRow[k + 1],
             triangleRow[k + 2]);

            // Set normal.
            glUniform3f(h_uNormal, cubeNormals[index/3].x, cubeNormals[index/3].y,
             cubeNormals[index/3].z);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT,
             (const GLvoid*)(getTriangleIndex(triangleRow[k],
             triangleRow[k + 1], triangleRow[k + 2]) * sizeof(short)));
         }

         ModelTrans.popMatrix();
      }

   } ModelTrans.popMatrix();

   // Disable the attributes used by our shader
   safe_glDisableVertexAttribArray(h_aPosition);

   //Disable the shader
   glUseProgram(0);
   glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y ){
   switch (key) {
   case 'l':
      modelAngle += 5.0f;
      break;

   case 'h':
      modelAngle -= 5.0f;
      break;

   case 'q': case 'Q' :
      exit(EXIT_SUCCESS);
      break;
   }

   glutPostRedisplay();
}

void reshape(int width, int height) {
	g_width = (float)width;
	g_height = (float)height;
	glViewport(0, 0, (GLsizei)width, (GLsizei)height);
}

void glut(int argc, char** argv) {
   glutInit(&argc, argv);
   glutInitWindowPosition(100, 100);
   glutInitWindowSize(800, 600);
   glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
   glutCreateWindow("Geometry Creator Demo");
   glutReshapeFunc(reshape);
   glutKeyboardFunc(keyboard);
   glutDisplayFunc(draw);
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

/**
 * Function to help load the shaders (both vertex and fragment.
 */
int InstallShader(const GLchar *vShaderName, const GLchar *fShaderName) {

   GLuint VS; //handles to shader object
   GLuint FS; //handles to frag shader object
   GLint vCompiled, fCompiled, linked; //status of shader

   VS = glCreateShader(GL_VERTEX_SHADER);
   FS = glCreateShader(GL_FRAGMENT_SHADER);

   //load the source
   glShaderSource(VS, 1, &vShaderName, NULL);
   glShaderSource(FS, 1, &fShaderName, NULL);

   //compile shader and print log
   glCompileShader(VS);
   /* check shader status requires helper functions */
   printOpenGLError();
   glGetShaderiv(VS, GL_COMPILE_STATUS, &vCompiled);
   printShaderInfoLog(VS);

   //compile shader and print log
   glCompileShader(FS);
   /* check shader status requires helper functions */
   printOpenGLError();
   glGetShaderiv(FS, GL_COMPILE_STATUS, &fCompiled);
   printShaderInfoLog(FS);

   if (!vCompiled || !fCompiled) {
      printf("Error compiling either shader %s or %s", vShaderName, fShaderName);
      return 0;
   }

   //create a program object and attach the compiled shader
   shaderProgram = glCreateProgram();
   glAttachShader(shaderProgram, VS);
   glAttachShader(shaderProgram, FS);

   glLinkProgram(shaderProgram);
   /* check shader status requires helper functions */
   printOpenGLError();
   glGetProgramiv(shaderProgram, GL_LINK_STATUS, &linked);
   printProgramInfoLog(shaderProgram);

   glUseProgram(shaderProgram);

   /* get handles to attribute data */
   h_aPosition = safe_glGetAttribLocation(shaderProgram, "aPosition");
   /* add a handle for the normal */
   h_uNormal = safe_glGetUniformLocation(shaderProgram, "uNormal");
   h_uProjMatrix = safe_glGetUniformLocation(shaderProgram, "uProjMatrix");
   h_uViewMatrix = safe_glGetUniformLocation(shaderProgram, "uViewMatrix");
   h_uModelMatrix = safe_glGetUniformLocation(shaderProgram, "uModelMatrix");

   printf("sucessfully installed shader %d\n", shaderProgram);
   return 1;
}

float implicitParaboloid(int x, int y, int z) {
   return x*x/4 + y*y/9 - z/2;
}

float implicitHyperbolicParaboloid(int x, int y, int z) {
   return y*y/9 - x*x/4 - z/2;
}

float implicitHyperboloid(int x, int y, int z) {
   return x*x/16 + y*y/9 - z*z/25 - 1;
}

float implicitEllipsoid(int x, int y, int z) {
   return x*x/1034 + y*y/5003 + z*z/1532 - 1;
}

float implicitSphere(int x, int y, int z) {
   return x*x + y*y + z*z - radius*radius;
}

int main(int argc, char** argv) {
   glut(argc, argv);

   marchingCubes.insideOutsideTest(implicitSphere, 1);
   marchingCubes.generateSurfaces();

   openGLInitialize();

   if (!InstallShader(textFileRead((char *)"src/marching.vert"),
    textFileRead((char *)"src/marching.frag"))) {
      cerr << "Error installing shader!" << endl;
      exit(EXIT_FAILURE);
   }

   InitCubeTriangles();

   glutMainLoop();

   return 0;
}
