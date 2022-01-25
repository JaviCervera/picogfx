#ifndef PICOGFX_MATH_H
#define PICOGFX_MATH_H

#include "common.h"

#ifdef __cplusplus
extern "C" {
#endif

void math_setortho(float left, float right, float top, float bottom, float near, float far, float* m);
void math_setperspective(float fov, float ratio, float start, float end, float* m);
void math_setview(float x, float y, float z, float pitch, float yaw, float roll, float* m);
void math_settransform(float x, float y, float z, float pitch, float yaw, float roll, float sx, float sy, float sz, float* m);
void math_setidentity(float* m);
void math_mulmatrices(const float* a, const float* b, float* m);
void math_mulvecbymatrix(const float* vec, const float* m, float* out);
void math_rotatevec(float x, float y, float z, float pitch, float yaw, float roll, float* out);
void math_lookat(float x, float y, float z, float target_x, float target_y, float target_z, float* out);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /* PICOGFX_MATH_H */
