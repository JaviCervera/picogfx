#include "../include/geom.h"
#include "../include/vertex.h"
#include "core_gles20.h"
#include "glad_gles20/include/glad/glad.h"
#include "renderdata_gles20.h"

GLuint _geom_create_vertex_buffer(const struct vertex_t* vertices, size_t num_vertices);
GLuint _geom_create_index_buffer(const unsigned short* indices, size_t num_indices);

struct geom_t {
  enum rendermode_t mode;
  GLuint vertex_buffer;
  GLuint index_buffer;
  size_t num_indices;
};

struct geom_t* geom_create(enum rendermode_t mode, const struct vertex_t* vertices, size_t num_vertices, const unsigned short* indices, size_t num_indices) {
  struct geom_t* geom = _alloc(struct geom_t);
  geom->mode = mode;
  geom->vertex_buffer = _geom_create_vertex_buffer(vertices, num_vertices);
  geom->index_buffer = _geom_create_index_buffer(indices, num_indices);
  geom->num_indices = num_indices;
  return geom;
}

GLuint _geom_create_vertex_buffer(const struct vertex_t* vertices, size_t num_vertices) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ARRAY_BUFFER, buffer);
  glBufferData(GL_ARRAY_BUFFER, num_vertices * sizeof(struct vertex_t), vertices, GL_STATIC_DRAW);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  return buffer;
}

GLuint _geom_create_index_buffer(const unsigned short* indices, size_t num_indices) {
  GLuint buffer;
  glGenBuffers(1, &buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, num_indices * sizeof(unsigned short), indices, GL_STATIC_DRAW);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
  return buffer;
}

void geom_destroy(struct geom_t* geom) {
  glDeleteBuffers(1, &geom->vertex_buffer);
  glDeleteBuffers(1, &geom->index_buffer);
  free(geom);
}

void geom_render(struct geom_t* geom, struct renderdata_t* renderdata, enum blendmode_t blend, bool_t culling, bool_t depth_test, bool_t depth_write) {
  static const GLenum glmode[] = {GL_POINTS, GL_LINES, GL_TRIANGLES, GL_TRIANGLE_STRIP, GL_TRIANGLE_FAN};
  _core_setblendmode(blend);
  _core_setculling(culling);
  _core_setdepthtest(depth_test);
  _core_setdepthwrite(depth_write);
  glBindBuffer(GL_ARRAY_BUFFER, geom->vertex_buffer);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, geom->index_buffer);
  _renderdata_prepare(renderdata);
  glDrawElements(glmode[geom->mode], geom->num_indices, GL_UNSIGNED_SHORT, 0);
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
