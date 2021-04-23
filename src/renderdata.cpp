#include "gfxdriver.h"
#include "renderdata.h"
#include "shader.h"
#include "stb/stretchy_buffer.h"
#include "texture.h"

namespace picogfx {

RenderData* RenderData::Create(const Shader& shader) {
    return new impl::RenderData((const impl::Shader&)shader);
}

namespace impl {

RenderData::RenderData(const Shader& shader) : mShader(shader), mBindings(NULL), mNumTextures(0) {
}

RenderData::~RenderData() {
    sb_free(mBindings);
}

void RenderData::Discard() {
    delete this;
}

const picogfx::Shader& RenderData::GetShader() const {
    return mShader;
}

void RenderData::BindInt(const char* name, const int* value) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, value, PrepareInt));
}

void RenderData::BindFloat(const char* name, const float* value) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, value, PrepareFloat));
}

void RenderData::BindVec2(const char* name, const float* vec) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, vec, PrepareVec2));
}

void RenderData::BindVec3(const char* name, const float* vec) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, vec, PrepareVec3));
}

void RenderData::BindVec4(const char* name, const float* vec) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, vec, PrepareVec4));
}

void RenderData::BindMat4(const char* name, const float* mat) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, mat, PrepareMat4));
}

void RenderData::BindTexture(const char* name, const picogfx::Texture** tex) {
    const int loc = GfxDriver::Get().GetShaderVarLocation(mShader.GetId(), name);
    if (loc != -1) sb_push(mBindings, BindingData(loc, tex, PrepareTex));
}

void RenderData::Prepare() {
    mNumTextures = 0;
    mShader.Prepare();
    for (size_t i = 0; i < sb_count(mBindings); ++i) {
        mBindings[i].Func(*this, mBindings[i], mShader);
    }
}

size_t RenderData::GetNumTextures() const {
    return mNumTextures;
}

void RenderData::IncNumTextures() {
    mNumTextures++;
}

void RenderData::PrepareInt(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderInt(shader.GetId(), binding.Location, *(int*)binding.Value);
}

void RenderData::PrepareFloat(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderFloat(shader.GetId(), binding.Location, *(float*)binding.Value);
}

void RenderData::PrepareVec2(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderVec2(shader.GetId(), binding.Location, (float*)binding.Value);
}

void RenderData::PrepareVec3(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderVec3(shader.GetId(), binding.Location, (float*)binding.Value);
}

void RenderData::PrepareVec4(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderVec4(shader.GetId(), binding.Location, (float*)binding.Value);
}

void RenderData::PrepareMat4(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderMat4(shader.GetId(), binding.Location, (float*)binding.Value);
}

void RenderData::PrepareTex(RenderData& renderData, const BindingData& binding, const Shader& shader) {
    GfxDriver::Get().SetShaderInt(shader.GetId(), binding.Location, renderData.GetNumTextures());
    const Texture* tex = *(const Texture**)binding.Value;
    GfxDriver::Get().BindTexture(tex ? tex->GetId() : 0, renderData.GetNumTextures());
    renderData.IncNumTextures();
}

} // namespace impl
} // namespace picogfx
