#include "../include/math.h"
#define LITE_MATH3D_IMPLEMENTATION
#include "litelibs/litemath3d.h"

void math_setortho(float left, float right, float top, float bottom, float near, float far, float* m) {
  const lmat4_t mat = lmat4_ortholh(left, right, bottom, top, near, far);
  memcpy(m, &mat, sizeof(mat));
}

void math_setperspective(float fov, float ratio, float start, float end, float* m) {
  const lmat4_t mat = lmat4_perspectivelh(lm_deg2rad(fov), ratio, start, end);
  memcpy(m, &mat, sizeof(mat));
}

void math_setview(float x, float y, float z, float pitch, float yaw, float roll, float* m) {
  const lquat_t q = lquat_fromeuler(lvec3_rad(lvec3(pitch, yaw, roll)));
  lmat4_t mat = lmat4_identity();
  mat = lmat4_rotate(mat, -lquat_angle(q), lquat_axis(q));
  mat = lmat4_translate(mat, lvec3(-x, -y, -z));
  memcpy(m, &mat, sizeof(mat));
}

void math_settransform(float x, float y, float z, float pitch, float yaw, float roll, float sx, float sy, float sz, float* m) {
  const lmat4_t mat = lmat4_transform(
    lvec3(x, y, z),
    lquat_fromeuler(lvec3_rad(lvec3(pitch, yaw, roll))),
    lvec3(sx, sy, sz));
  memcpy(m, &mat, sizeof(mat));
}

void math_setidentity(float* m) {
  const lmat4_t mat = lmat4_identity();
  memcpy(m, &mat, sizeof(mat));
}

void math_mulmatrices(const float* a, const float* b, float* m) {
  const lmat4_t mat = lmat4_mul(*(const lmat4_t*)a, *(const lmat4_t*)b);
  memcpy(m, &mat, sizeof(mat));
}

void math_mulvecbymatrix(const float* vec, const float* m, float* out) {
  const lvec3_t result = lmat4_mulvec3(*(const lmat4_t*)m, lvec3(vec[0], vec[1], vec[2]), vec[3]);
  memcpy(out, &result, sizeof(result));
}

void math_rotatevec(float x, float y, float z, float pitch, float yaw, float roll, float* out) {
  const lvec3_t result = lquat_mulvec3(lquat_fromeuler(lvec3_rad(lvec3(pitch, yaw, roll))), lvec3(x, y, z));
  memcpy(out, &result, sizeof(result));
}

void math_lookat(float x, float y, float z, float target_x, float target_y, float target_z, float* out) {
  const lvec3_t dir = lvec3_norm(lvec3_sub(lvec3(x, y, z), lvec3(target_x, target_y, target_z)));
  out[0] = lm_rad2deg((float)asin(-dir.y));
  out[1] = lm_rad2deg((float)atan2(dir.x, dir.z));
}
