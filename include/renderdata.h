/**
 * @file
 */

#ifndef PICOGFX_RENDERDATA_H
#define PICOGFX_RENDERDATA_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

struct renderdata_t;
struct shader_t;
struct texture_t;

struct renderdata_t* renderdata_create(struct shader_t* shader);
void renderdata_destroy(struct renderdata_t* renderdata);
struct shader_t* renderdata_shader(struct renderdata_t* renderdata);
void renderdata_bindint(struct renderdata_t* renderdata, const char* name, const int* value);
void renderdata_bindfloat(struct renderdata_t* renderdata, const char* name, const float* value);
void renderdata_bindvec2(struct renderdata_t* renderdata, const char* name, const float* value);
void renderdata_bindvec3(struct renderdata_t* renderdata, const char* name, const float* value);
void renderdata_bindvec4(struct renderdata_t* renderdata, const char* name, const float* value);
void renderdata_bindmat4(struct renderdata_t* renderdata, const char* name, const float* value);
void renderdata_bindtexture(struct renderdata_t* renderdata, const char* name, const struct texture_t** value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_RENDERDATA_H */
