#include "MarchingCubes.h"

MarchingCubes::MarchingCubes(vec3 size_) : size(size_) {
   points.assign(size.x * size.y * size.z, false);
}

int MarchingCubes::index(int x, int y, int z) {
   return (z * size.x * size.y) + (y * size.x) + x;
}
