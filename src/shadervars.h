#pragma once

#include "../include/shadervars.h"

namespace picogfx {
namespace impl {

struct ShaderVars : public picogfx::ShaderVars {
    ShaderVars(const Shader& shader);
    ~ShaderVars();
    virtual void Discard();
    virtual const Shader& GetShader() const;
    virtual void SetInt(const char* name, const int* value);
    virtual void SetFloat(const char* name, const float* value);
    virtual void SetVec2(const char* name, const float* vec);
    virtual void SetVec3(const char* name, const float* vec);
    virtual void SetVec4(const char* name, const float* vec);
    virtual void SetMat4(const char* name, const float* mat);
    virtual void SetTexture(const char* name, const picogfx::Texture* tex);
    void Prepare();
    size_t GetNumTextures() const;
    void IncNumTextures();
private:
    struct ShaderVar {
        typedef void (* PrepareFunc)(ShaderVars&, const ShaderVar&, const Shader&);
        size_t Location;
        const void* Value;
        PrepareFunc Func;
        ShaderVar(size_t location, const void* value, PrepareFunc func) : Location(location), Value(value), Func(func) {}
    };

    const Shader& mShader;
    ShaderVar* mVars;
    size_t mNumTextures;

    static void PrepareInt(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
    static void PrepareFloat(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
    static void PrepareVec2(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
    static void PrepareVec3(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
    static void PrepareVec4(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
    static void PrepareMat4(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
    static void PrepareTex(ShaderVars& shaderVars, const ShaderVar& var, const Shader& shader);
};

} // namespace impl
} // namespace picogfx
