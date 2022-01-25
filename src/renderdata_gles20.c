#include "glad_gles20/include/glad/glad.h"
#include "renderdata_gles20.h"
#include "shader_gles20.h"
#include "stb/stb_ds.h"
#include "texture_gles20.h"

struct bindingdata_t;

void _bindingdata_prepare_int(const struct bindingdata_t* bd, struct renderdata_t* rd);
void _bindingdata_prepare_float(const struct bindingdata_t* bd, struct renderdata_t* rd);
void _bindingdata_prepare_vec2(const struct bindingdata_t* bd, struct renderdata_t* rd);
void _bindingdata_prepare_vec3(const struct bindingdata_t* bd, struct renderdata_t* rd);
void _bindingdata_prepare_vec4(const struct bindingdata_t* bd, struct renderdata_t* rd);
void _bindingdata_prepare_mat4(const struct bindingdata_t* bd, struct renderdata_t* rd);
void _bindingdata_prepare_texture(const struct bindingdata_t* bd, struct renderdata_t* rd);


typedef void (* prepare_func_t)(const struct bindingdata_t*, struct renderdata_t*);

struct bindingdata_t {
  size_t location;
  const void* value;
  prepare_func_t func;
};

struct renderdata_t {
  struct shader_t* shader;
  struct bindingdata_t* bindings;
  size_t num_textures;
};

struct renderdata_t* renderdata_create(struct shader_t* shader) {
  struct renderdata_t* rd = _alloc(struct renderdata_t);
  rd->shader = shader;
  rd->bindings = NULL;
  rd->num_textures = 0;
  return rd;
}

void renderdata_destroy(struct renderdata_t* renderdata) {
  arrfree(renderdata->bindings);
  free(renderdata);
}

struct shader_t* renderdata_shader(struct renderdata_t* renderdata) {
  return renderdata->shader;
}

void renderdata_bindint(struct renderdata_t* renderdata, const char* name, const int* value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_int};
    arrput(renderdata->bindings, bindingdata);
  }
}

void renderdata_bindfloat(struct renderdata_t* renderdata, const char* name, const float* value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_float};
    arrput(renderdata->bindings, bindingdata);
  }
}

void renderdata_bindvec2(struct renderdata_t* renderdata, const char* name, const float* value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_vec2};
    arrput(renderdata->bindings, bindingdata);
  }
}

void renderdata_bindvec3(struct renderdata_t* renderdata, const char* name, const float* value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_vec3};
    arrput(renderdata->bindings, bindingdata);
  }
}

void renderdata_bindvec4(struct renderdata_t* renderdata, const char* name, const float* value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_vec4};
    arrput(renderdata->bindings, bindingdata);
  }
}

void renderdata_bindmat4(struct renderdata_t* renderdata, const char* name, const float* value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_mat4};
    arrput(renderdata->bindings, bindingdata);
  }
}

void renderdata_bindtexture(struct renderdata_t* renderdata, const char* name, const struct texture_t** value) {
  const int loc = glGetUniformLocation(_shader_id(renderdata->shader), name);
  if (loc != -1) {
    const struct bindingdata_t bindingdata = {loc, value, _bindingdata_prepare_texture};
    arrput(renderdata->bindings, bindingdata);
  }
}

void _renderdata_prepare(struct renderdata_t* renderdata) {
  renderdata->num_textures = 0;
  _shader_prepare(renderdata->shader);
  for (size_t i = 0; i < arrlenu(renderdata->bindings); ++i) {
    struct bindingdata_t* bd = &renderdata->bindings[i];
    bd->func(bd, renderdata);
  }
}

void _bindingdata_prepare_int(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  glUniform1i(bd->location, *(int*)bd->value);
}

void _bindingdata_prepare_float(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  glUniform1f(bd->location, *(float*)bd->value);
}

void _bindingdata_prepare_vec2(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  glUniform2fv(bd->location, 1, (float*)bd->value);
}

void _bindingdata_prepare_vec3(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  glUniform3fv(bd->location, 1, (float*)bd->value);
}

void _bindingdata_prepare_vec4(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  glUniform4fv(bd->location, 1, (float*)bd->value);
}

void _bindingdata_prepare_mat4(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  glUniformMatrix4fv(bd->location, 1, FALSE, (float*)bd->value);
}

void _bindingdata_prepare_texture(const struct bindingdata_t* bd, struct renderdata_t* rd) {
  const struct texture_t* texture = *(const struct texture_t**)bd->value;
  glUniform1i(bd->location, rd->num_textures);
  _texture_bind(texture, rd->num_textures);
  ++rd->num_textures;
}
