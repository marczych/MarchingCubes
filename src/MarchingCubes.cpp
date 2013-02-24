#include "MarchingCubes.h"

#include <iostream>

MarchingCubes::MarchingCubes(vec3 size_) : size(size_) {
   points.resize((size.x + 1) * (size.y + 1) * (size.z + 1));
}

void MarchingCubes::insideOutsideTest(float (*equation) (int, int, int), float isoValue) {
   for (int x = 0; x < size.x + 1; x++) {
      for (int y = 0; y < size.y + 1; y++) {
         for (int z = 0; z < size.z + 1; z++) {
            points[index(x, y, z)] = equation(x - size.x/2, y - size.y/2,
             z - size.z/2) < isoValue;
         }
      }
   }
}

int MarchingCubes::index(int x, int y, int z) {
   return (z * size.x * size.y) + (y * size.x) + x;
}
