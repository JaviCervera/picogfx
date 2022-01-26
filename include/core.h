/**
 * @file
 */

#ifndef PICOGFX_CORE_H
#define PICOGFX_CORE_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef void* (* load_proc_t)(const char*);

bool_t core_init(load_proc_t proc);
void core_prepare(int viewport_x, int viewport_y, int viewport_width, int viewport_height, int color);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_CORE_H */
