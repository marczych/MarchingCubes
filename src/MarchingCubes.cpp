#include "MarchingCubes.h"

MarchingCubes::MarchingCubes(vec3 size_) : size(size_) {
   points.resize((size.x + 1) * (size.y + 1) * (size.z + 1));
}

int MarchingCubes::index(int x, int y, int z) {
   return (z * size.x * size.y) + (y * size.x) + x;
}
