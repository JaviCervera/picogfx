#pragma once

namespace picogfx {

struct Shader;
struct Texture;

struct RenderData {
    static RenderData* Create(const Shader& shader);
    virtual void Discard() = 0;
    virtual const Shader& GetShader() const = 0;
    virtual void BindInt(const char* name, const int* value) = 0;
    virtual void BindFloat(const char* name, const float* value) = 0;
    virtual void BindVec2(const char* name, const float* vec) = 0;
    virtual void BindVec3(const char* name, const float* vec) = 0;
    virtual void BindVec4(const char* name, const float* vec) = 0;
    virtual void BindMat4(const char* name, const float* mat) = 0;
    virtual void BindTexture(const char* name, const Texture** tex) = 0;
};

} // namespace picogfx
