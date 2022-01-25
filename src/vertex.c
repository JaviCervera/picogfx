#include "../include/vertex.h"

void vertex_init(struct vertex_t* vertex, float x, float y, float z, int color, float u, float v) {
  vertex_init_ex(vertex, x, y, z, 0, 0, -1, 1, 0, 0, color, u, v, u, v);
}

void vertex_init_ex(struct vertex_t* vertex, float x, float y, float z, float nx, float ny, float nz, float tx, float ty, float tz, int color, float u, float v, float u1, float v1) {
  vertex->position[0] = x;
  vertex->position[1] = y;
  vertex->position[2] = z;
  vertex->normal[0] = nx;
  vertex->normal[1] = ny;
  vertex->normal[2] = nz;
  vertex->tangent[0] = tx;
  vertex->tangent[1] = ty;
  vertex->tangent[2] = tz;
  vertex->color = color;
  vertex->texcoords[0] = u;
  vertex->texcoords[1] = v;
  vertex->texcoords[2] = u1;
  vertex->texcoords[3] = v1;
}
