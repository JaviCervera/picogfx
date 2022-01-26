/**
 * @file
 */

#ifndef PICOGFX_TEXTURE_H
#define PICOGFX_TEXTURE_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

enum texturefilter_t {
  tf_none,
  tf_linear,
  tf_bilinear,
  tf_trilinear
};

struct texture_t;

struct texture_t* texture_create(size_t width, size_t height, enum texturefilter_t filter, bool_t isdepth);
void texture_destroy(struct texture_t* texture);
void texture_setpixels(struct texture_t* texture, const unsigned char* pixels);
size_t texture_width(const struct texture_t* texture);
size_t texture_height(const struct texture_t* texture);
enum texturefilter_t texture_filter(const struct texture_t* texture);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_TEXTURE_H */
