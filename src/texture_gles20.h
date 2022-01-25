#ifndef PICOGFX_TEXTURE_GLES20_H
#define PICOGFX_TEXTURE_GLES20_H

#include "../include/texture.h"

#ifdef __cplusplus
extern "C" {
#endif

void _texture_bind(const struct texture_t* texture, size_t unit);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_TEXTURE_GLES20_H */
