/**
 * @file
 */

#ifndef PICOGFX_SHADER_H
#define PICOGFX_SHADER_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct shader_t;

struct shader_t* shader_create(const char* vertex, const char* fragment, char* out_error, size_t out_size);
void shader_destroy(struct shader_t* shader);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_SHADER_H */
