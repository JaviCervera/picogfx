#pragma once

#include "../include/renderdata.h"

namespace picogfx {
namespace impl {

struct Shader;

struct RenderData : public picogfx::RenderData {
    RenderData(const Shader& shader);
    ~RenderData();
    virtual void Discard();
    virtual const picogfx::Shader& GetShader() const;
    virtual void BindInt(const char* name, const int* value);
    virtual void BindFloat(const char* name, const float* value);
    virtual void BindVec2(const char* name, const float* vec);
    virtual void BindVec3(const char* name, const float* vec);
    virtual void BindVec4(const char* name, const float* vec);
    virtual void BindMat4(const char* name, const float* mat);
    virtual void BindTexture(const char* name, const picogfx::Texture** tex);
    void Prepare();
    size_t GetNumTextures() const;
    void IncNumTextures();
private:
    struct BindingData {
        typedef void (* PrepareFunc)(RenderData&, const BindingData&, const Shader&);
        size_t Location;
        const void* Value;
        PrepareFunc Func;
        BindingData(size_t location, const void* value, PrepareFunc func) : Location(location), Value(value), Func(func) {}
    };

    const Shader& mShader;
    BindingData* mBindings;
    size_t mNumTextures;

    static void PrepareInt(RenderData& renderData, const BindingData& binding, const Shader& shader);
    static void PrepareFloat(RenderData& renderData, const BindingData& binding, const Shader& shader);
    static void PrepareVec2(RenderData& renderData, const BindingData& binding, const Shader& shader);
    static void PrepareVec3(RenderData& renderData, const BindingData& binding, const Shader& shader);
    static void PrepareVec4(RenderData& renderData, const BindingData& binding, const Shader& shader);
    static void PrepareMat4(RenderData& renderData, const BindingData& binding, const Shader& shader);
    static void PrepareTex(RenderData& renderData, const BindingData& binding, const Shader& shader);
};

} // namespace impl
} // namespace picogfx
