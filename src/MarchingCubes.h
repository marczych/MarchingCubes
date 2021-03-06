#ifndef __MARCHING_CUBES_H__
#define __MARCHING_CUBES_H__

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class MarchingCubes {
public:
   MarchingCubes(vec3 size_);

   void insideOutsideTest(float (*equation) (int x, int y, int z), float);
   void generateSurfaces();
   ivec3 coordinate(int idx);

   const vector<unsigned char>& getCubes() { return cubes; }

   const static int edgeTable[256];
   const static int triTable[256][16];

private:
   ivec3 size; // Size of marching cubes grid.
   vector<bool> points; // 3-dimensional list for inside/outside.
   vector<unsigned char> cubes; // 3-dimensional list for voxel grid.

   int index(int x, int y, int z);
};

#endif
