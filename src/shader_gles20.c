#include "../include/vertex.h"
#include "glad_gles20/include/glad/glad.h"
#include "shader_gles20.h"

struct shaderattribs_t {
  int vertex_position;
  int vertex_normal;
  int vertex_tangent;
  int vertex_color;
  int vertex_texcoords;
};

void _shaderattribs_init(struct shaderattribs_t* attribs, GLuint shader);
void _shaderattribs_prepare(struct shaderattribs_t* attribs, GLuint shader);
void _shader_setvar(GLuint shader, int location, size_t size, bool_t normalize, size_t offset);

struct shader_t {
  GLuint id;
  struct shaderattribs_t attribs;
};

struct shader_t* shader_create(const char* vertex, const char* fragment, char* out_error, size_t out_size) {
  GLint ret_code;

  /* create vertex shader */
  const GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
  glShaderSource(vshader, 1, &vertex, NULL);
  glCompileShader(vshader);
  glGetShaderiv(vshader, GL_COMPILE_STATUS, &ret_code);
  if (ret_code == GL_FALSE) {
      if (out_error) glGetShaderInfoLog(vshader, out_size, NULL, out_error);
      glDeleteShader(vshader);
      return NULL;
  }

  /* create fragment shader */
  const GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
  glShaderSource(fshader, 1, &fragment, NULL);
  glCompileShader(fshader);
  glGetShaderiv(fshader, GL_COMPILE_STATUS, &ret_code);
  if (ret_code == GL_FALSE) {
      if (out_error) glGetShaderInfoLog(fshader, out_size, NULL, out_error);
      glDeleteShader(vshader);
      glDeleteShader(fshader);
      return NULL;
  }

  /* create program */
  const size_t id = glCreateProgram();
  glAttachShader(id, vshader);
  glAttachShader(id, fshader);
  glLinkProgram(id);
  glDeleteShader(vshader);
  glDeleteShader(fshader);
  glGetProgramiv(id, GL_LINK_STATUS, &ret_code);
  if (ret_code == GL_FALSE) {
      if (out_error) glGetProgramInfoLog(id, out_size, NULL, out_error);
      glDeleteProgram(id);
      return NULL;
  }

  struct shader_t* shader = _alloc(struct shader_t);
  shader->id = id;
  _shaderattribs_init(&shader->attribs, id);
  return shader;
}

void shader_destroy(struct shader_t* shader) {
  glDeleteProgram(shader->id);
  free(shader);
}

size_t _shader_id(struct shader_t* shader) {
  return shader->id;
}

void _shader_prepare(struct shader_t* shader) {
  glUseProgram(shader->id);
  _shaderattribs_prepare(&shader->attribs, shader->id);
}

void _shaderattribs_init(struct shaderattribs_t* attribs, GLuint shader) {
  attribs->vertex_position = glGetAttribLocation(shader, "vertex_position");
  attribs->vertex_normal = glGetAttribLocation(shader, "vertex_normal");
  attribs->vertex_tangent = glGetAttribLocation(shader, "vertex_tangent");
  attribs->vertex_color = glGetAttribLocation(shader, "vertex_color");
  attribs->vertex_texcoords = glGetAttribLocation(shader, "vertex_texcoords");
}

void _shaderattribs_prepare(struct shaderattribs_t* attribs, GLuint shader) {
  _shader_setvar(shader, attribs->vertex_position, 3, FALSE, offsetof(struct vertex_t, position));
  _shader_setvar(shader, attribs->vertex_normal, 3, FALSE, offsetof(struct vertex_t, normal));
  _shader_setvar(shader, attribs->vertex_tangent, 3, FALSE, offsetof(struct vertex_t, tangent));
  _shader_setvar(shader, attribs->vertex_color, 4, TRUE, offsetof(struct vertex_t, color));
  _shader_setvar(shader, attribs->vertex_texcoords, 4, FALSE, offsetof(struct vertex_t, texcoords));
}

void _shader_setvar(GLuint shader, int location, size_t size, bool_t normalize, size_t offset) {
  if (location != -1) {
    glEnableVertexAttribArray(location);
    glVertexAttribPointer(
      location,
      size,
      normalize ? GL_UNSIGNED_BYTE : GL_FLOAT,
      normalize,
      sizeof(struct vertex_t),
      (const void*)offset);
  }
}
