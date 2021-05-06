#pragma once

#include "../include/core.h"

namespace picogfx {
namespace impl {

struct Core : public picogfx::Core {
    virtual bool Init(LoadProc proc);
    virtual void SetOrtho(float left, float right, float top, float bottom, float near, float far, float* m);
    virtual void SetPerspective(float fov, float ratio, float start, float end, float* m);
    virtual void SetView(float x, float y, float z, float pitch, float yaw, float roll, float* m);
    virtual void SetTransform(float x, float y, float z, float pitch, float yaw, float roll, float sx, float sy, float sz, float* m);
    virtual void SetIdentity(float* m);
    virtual void MulMatrices(const float* a, const float* b, float* m);
    virtual void MulVecByMatrix(const float* vec, const float* m, float* out);
    virtual void RotateVec(float x, float y, float z, float pitch, float yaw, float roll, float* out);
    virtual void Prepare(int viewportX, int viewportY, int viewportWidth, int viewportHeight, int color = 0);
};

} // namespace impl
} // namespace picogfx
