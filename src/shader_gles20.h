#ifndef PICOGFX_SHADER_GLES20_H
#define PICOGFX_SHADER_GLES20_H

#include "../include/shader.h"

#ifdef __cplusplus
extern "C" {
#endif

size_t _shader_id(struct shader_t* shader);
void _shader_prepare(struct shader_t* shader);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_SHADER_GLES20_H */
