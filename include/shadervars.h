#pragma once

namespace picogfx {

struct Shader;
struct Texture;

struct ShaderVars {
    static ShaderVars* Create(const Shader& shader);
    virtual void Discard() = 0;
    virtual const Shader& GetShader() const = 0;
    virtual void SetInt(const char* name, const int* value) = 0;
    virtual void SetFloat(const char* name, const float* value) = 0;
    virtual void SetVec2(const char* name, const float* vec) = 0;
    virtual void SetVec3(const char* name, const float* vec) = 0;
    virtual void SetVec4(const char* name, const float* vec) = 0;
    virtual void SetMat4(const char* name, const float* mat) = 0;
    virtual void SetTexture(const char* name, const Texture* tex) = 0;
};

} // namespace picogfx
