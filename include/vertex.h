#ifndef PICOGFX_VERTEX_H
#define PICOGFX_VERTEX_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct vertex_t {
  float position[3];
  float normal[3];
  float tangent[3];
  int color;
  float texcoords[4];
};

void vertex_init(struct vertex_t* vertex, float x, float y, float z, int color, float u, float v);
void vertex_init_ex(struct vertex_t* vertex, float x, float y, float z, float nx, float ny, float nz, float tx, float ty, float tz, int color, float u, float v, float u1, float v1);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_VERTEX_H */
