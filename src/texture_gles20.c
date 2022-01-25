#include "glad_gles20/include/glad/glad.h"
#include "texture_gles20.h"

GLint _texture_minfilter(enum texturefilter_t filter);
GLint _texture_magfilter(enum texturefilter_t filter);

struct texture_t {
  GLuint id;
  size_t width;
  size_t height;
  enum texturefilter_t filter;
};

struct texture_t* texture_create(size_t width, size_t height, enum texturefilter_t filter, bool_t isdepth) {
  struct texture_t* tex = _alloc(struct texture_t);
  tex->width = width;
  tex->height = height;
  tex->filter = filter;
  glGenTextures(1, &tex->id);
  glBindTexture(GL_TEXTURE_2D, tex->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(
    GL_TEXTURE_2D,
    0,
    isdepth ? GL_DEPTH_COMPONENT16 : GL_RGBA,
    width,
    height,
    0,
    isdepth ? GL_DEPTH_COMPONENT : GL_RGBA,
    GL_UNSIGNED_BYTE,
    NULL);
  glBindTexture(GL_TEXTURE_2D, 0);
  return tex;
}

void texture_destroy(struct texture_t* texture) {
  glDeleteTextures(1, &texture->id);
  free(texture);
}

void texture_setpixels(struct texture_t* texture, const unsigned char* pixels) {
  glBindTexture(GL_TEXTURE_2D, texture->id);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, _texture_minfilter(texture->filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, _texture_magfilter(texture->filter));
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture->width, texture->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
  if (texture->filter > tf_linear) glGenerateMipmap(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, 0);
}

size_t texture_width(const struct texture_t* texture) {
  return texture->width;
}

size_t texture_height(const struct texture_t* texture) {
  return texture->height;
}

enum texturefilter_t texture_filter(const struct texture_t* texture) {
  return texture->filter;
}

void _texture_bind(const struct texture_t* texture, size_t unit) {
  glActiveTexture(GL_TEXTURE0 + unit);
  glBindTexture(GL_TEXTURE_2D, texture->id);
}

GLint _texture_minfilter(enum texturefilter_t filter) {
  switch (filter) {
    case tf_none:
      return GL_NEAREST;
    case tf_linear:
      return GL_LINEAR;
    case tf_bilinear:
      return GL_LINEAR;
    case tf_trilinear:
      return GL_LINEAR_MIPMAP_LINEAR;
    default:
      return GL_LINEAR;
  }
}

GLint _texture_magfilter(enum texturefilter_t filter) {
  return (filter == tf_none)
    ? GL_NEAREST
    : GL_LINEAR;
}
