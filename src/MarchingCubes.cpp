#include "MarchingCubes.h"

MarchingCubes::MarchingCubes(vec3 size_) : size(size_) {
   points.resize(size.x * size.y * size.z);
}

int MarchingCubes::index(int x, int y, int z) {
   return (z * size.x * size.y) + (y * size.x) + x;
}
