#include "gfxdriver.h"
#include "shadervars.h"
#include "stb/stretchy_buffer.h"

namespace picogfx {

ShaderVars* ShaderVars::Create(const Shader& shader) {
    return new ShaderVars(shader);
}

namespace impl {

ShaderVars::ShaderVars(const Shader& shader) : mShader(shader), mVars(NULL), mNumTextures(0) {
}

ShaderVars::~ShaderVars() {
    sb_free(mVars);
}

void ShaderVars::Discard() {
    delete this;
}

Shader& ShaderVars::GetShader() {
    return mShader;
}

void ShaderVars::SetInt(const char* name, int value) {
    void PrepareInt(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderInt(shader->GetId(), *(int*)var.Value);
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareInt));
}

void ShaderVars::SetFloat(const char* name, float value) {
    void PrepareFloat(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderFloat(shader->GetId(), *(float*)var.Value);
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareFloat));
}

void ShaderVars::SetVec2(const char* name, const float* vec) {
    void PrepareVec2(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderVec2(shader->GetId(), (float*)var.Value);
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareVec2));
}

void ShaderVars::SetVec3(const char* name, const float* vec) {
    void PrepareVec3(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderVec3(shader->GetId(), (float*)var.Value);
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareVec3));
}

void ShaderVars::SetVec4(const char* name, const float* vec) {
    void PrepareVec3(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderVec4(shader->GetId(), (float*)var.Value);
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareVec4));
}

void ShaderVars::SetMat4(const char* name, const float* mat) {
    void PrepareVec3(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderMat3(shader->GetId(), (float*)var.Value);
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareMat4));
}

void ShaderVars::SetTexture(const char* name, const Texture* tex) {
    void PrepareTex(const ShaderVars& settings, const ShaderVar& var, const Shader& shader) {
        GfxDriver::Get().SetShaderInt(shader->GetId(), mNumTextures);
        GfxDriver::Get().SetTexture(tex->GetId(), mNumTextures);
        mNumTextures++;
    }
    const size_t loc = GfxDriver::Get().GetShaderLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareTex));
}

void ShaderVars::Prepare() {
    mNumTextures = 0;
    mShader.Prepare();
    for (size_t i = 0; i < sb_count(mVars); ++i) {
        Func(*this, mVars[i], shader);
    }
}

} // namespace impl
} // namespace picogfx
