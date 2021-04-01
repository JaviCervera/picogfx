#pragma once

#include "../include/shadervars.h"

namespace picogfx {
namespace impl {

struct ShaderVars : public picogfx::ShaderVars {
    ShaderVars(const Shader& shader);
    ~ShaderVars();
    virtual void Discard();
    virtual Shader& GetShader();
    virtual void SetInt(const char* name, const int* value);
    virtual void SetFloat(const char* name, const float* value);
    virtual void SetVec2(const char* name, const float* vec);
    virtual void SetVec3(const char* name, const float* vec);
    virtual void SetVec4(const char* name, const float* vec);
    virtual void SetMat4(const char* name, const float* mat);
    virtual void SetTexture(const char* name, const Texture* tex);
    void Prepare();
private:
    struct ShaderVar {
        typedef void (* PrepareFunc)(const ShaderVars&, const ShaderVar&, const Shader&);
        size_t Location;
        const void* Value;
        PrepareFunc Func;
        ShaderVar(size_t location, const void* value, PrepareFunc func) : Location(location), Value(value), Func(func) {}
    };

    Shader& mShader;
    ShaderVar* mVars;
    size_t mNumTextures;
};

} // namespace impl
} // namespace picogfx
