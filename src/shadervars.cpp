#include "gfxdriver.h"
#include "shader.h"
#include "shadervars.h"
#include "stb/stretchy_buffer.h"
#include "texture.h"

namespace picogfx {

ShaderVars* ShaderVars::Create(const Shader& shader) {
    return new impl::ShaderVars((const impl::Shader&)shader);
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

const Shader& ShaderVars::GetShader() const {
    return mShader;
}

void ShaderVars::SetInt(const char* name, const int* value) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareInt));
}

void ShaderVars::SetFloat(const char* name, const float* value) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, value, PrepareFloat));
}

void ShaderVars::SetVec2(const char* name, const float* vec) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, vec, PrepareVec2));
}

void ShaderVars::SetVec3(const char* name, const float* vec) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, vec, PrepareVec3));
}

void ShaderVars::SetVec4(const char* name, const float* vec) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, vec, PrepareVec4));
}

void ShaderVars::SetMat4(const char* name, const float* mat) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, mat, PrepareMat4));
}

void ShaderVars::SetTexture(const char* name, const picogfx::Texture* tex) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mVars, ShaderVar(loc, tex, PrepareTex));
}

void ShaderVars::Prepare() {
    mNumTextures = 0;
    mShader.Prepare();
    for (size_t i = 0; i < sb_count(mVars); ++i) {
        mVars[i].Func(*this, mVars[i], mShader);
    }
}

size_t ShaderVars::GetNumTextures() const {
    return mNumTextures;
}

void ShaderVars::IncNumTextures() {
    mNumTextures++;
}

void ShaderVars::PrepareInt(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderInt(shader.GetId(), var.Location, *(int*)var.Value);
}

void ShaderVars::PrepareFloat(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderFloat(shader.GetId(), var.Location, *(float*)var.Value);
}

void ShaderVars::PrepareVec2(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderVec2(shader.GetId(), var.Location, (float*)var.Value);
}

void ShaderVars::PrepareVec3(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderVec3(shader.GetId(), var.Location, (float*)var.Value);
}

void ShaderVars::PrepareVec4(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderVec4(shader.GetId(), var.Location, (float*)var.Value);
}

void ShaderVars::PrepareMat4(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderMat4(shader.GetId(), var.Location, (float*)var.Value);
}

void ShaderVars::PrepareTex(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader) {
    GfxDriver::Get().SetShaderInt(shader.GetId(), var.Location, shaderVars.GetNumTextures());
    GfxDriver::Get().BindTexture(((const Texture*)var.Value)->GetId(), shaderVars.GetNumTextures());
    shaderVars.IncNumTextures();
}

} // namespace impl
} // namespace picogfx
