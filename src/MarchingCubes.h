#ifndef __MARCHING_CUBES_H__
#define __MARCHING_CUBES_H__

#include <glm/glm.hpp>
#include <vector>

using namespace glm;
using namespace std;

class MarchingCubes {
public:
   MarchingCubes(vec3 size_);

private:
   vec3 size; // Size of marching cubes grid.
   vector<bool> points; // 3-dimensional list for inside/outside.

   int index(int x, int y, int z);
};

#endif
