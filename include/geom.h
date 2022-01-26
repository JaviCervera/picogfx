/**
 * @file
 */

#ifndef PICOGFX_GEOM_H
#define PICOGFX_GEOM_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

enum blendmode_t {
  bm_solid,
  bm_alpha,
  bm_add,
  bm_multiply
};

enum rendermode_t {
  rm_points,
  rm_lines,
  rm_triangles,
  rm_triangle_strip,
  rm_triangle_fan
};

struct geom_t;
struct renderdata_t;
struct vertex_t;

struct geom_t* geom_create(enum rendermode_t mode, const struct vertex_t* vertices, size_t num_vertices, const unsigned short* indices, size_t num_indices);
void geom_destroy(struct geom_t* geom);
void geom_render(struct geom_t* geom, struct renderdata_t* renderdata, enum blendmode_t blend, bool_t culling, bool_t depth_test, bool_t depth_write);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_GEOM_H */
