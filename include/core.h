#pragma once

#include <stddef.h>

namespace picogfx {

struct Core {
    typedef void* (* LoadProc)(const char*);

    static Core& Get();
    virtual bool Init(LoadProc proc) = 0;
    virtual void SetOrtho(float left, float right, float top, float bottom, float near, float far, float* m) = 0;
    virtual void SetPerspective(float fov, float ratio, float start, float end, float* m) = 0;
    virtual void SetView(float x, float y, float z, float pitch, float yaw, float roll, float* m) = 0;
    virtual void SetTransform(float x, float y, float z, float pitch, float yaw, float roll, float sx, float sy, float sz, float* m) = 0;
    virtual void SetIdentity(float* m) = 0;
    virtual void MulMatrices(const float* a, const float* b, float* m) = 0;
    virtual void MulVecByMatrix(const float* vec, const float* m, float* out);
    virtual void Prepare(int viewportX, int viewportY, int viewportWidth, int viewportHeight, int color = 0) = 0;
};

} // namespace picogfx
