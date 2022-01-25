#ifndef PICOGFX_CORE_GLES20_H
#define PICOGFX_CORE_GLES20_H

#include "../include/core.h"

#ifdef __cplusplus
extern "C" {
#endif

void _core_setblendmode(enum blendmode_t blend);
void _core_setculling(bool_t enable);
void _core_setdepthtest(bool_t enable);
void _core_setdepthwrite(bool_t enable);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_CORE_GLES20_H */
