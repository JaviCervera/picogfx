#include "core.h"
#include "gfxdriver.h"
#define LITE_MATH3D_IMPLEMENTATION
#include "litelibs/litemath3d.h"

namespace picogfx {
namespace impl {

bool Core::Init(LoadProc proc) {
    return GfxDriver::Get().Init(proc);
}

void Core::SetPerspective(float fov, float ratio, float start, float end, float* m) {
    lmat4_t mat = lmat4_perspectivelh(lm_deg2rad(fov), ratio, start, end);
    memcpy(m, &mat, sizeof(mat));
}

void Core::SetView(float x, float y, float z, float pitch, float yaw, float roll, float* m) {
    lquat_t q = lquat_fromeuler(lvec3_rad(lvec3(pitch, yaw, roll)));
    lmat4_t view = lmat4_identity();
    view = lmat4_rotate(view, -lquat_angle(q), lquat_axis(q));
    view = lmat4_translate(view, lvec3(-x, -y, -z));
}

void Core::SetTransform(float x, float y, float z, float pitch, float yaw, float roll, float sx, float sy, float sz, float* m) {
    lmat4_t mat = lmat4_transform(lvec3(x, y, z), lquat_fromeuler(lvec3_rad(lvec3(pitch, yaw, roll))), lvec3(sx, sy, sz));
    memcpy(m, &mat, sizeof(mat));
}

void Core::MulMatrices(const float* a, const float* b, float* m) {
    lmat4_t mat = lmat4_mul(*(const lmat4_t*)a, *(const lmat4_t*)b);
    memcpy(m, &mat, sizeof(mat));
}

void Core::Prepare(int viewportX, int viewportY, int viewportWidth, int viewportHeight, int color) {
    GfxDriver::Get().Prepare(viewportX, viewportY, viewportWidth, viewportHeight, color);
}

} // namespace impl

Core& Core::Get() {
    static Core* instance = new impl::Core();
    return *instance;
}

} // namespace picogfx
