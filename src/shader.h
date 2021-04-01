#pragma once

#include "../include/shader.h"

namespace picogfx {
namespace impl {

struct Shader : public picogfx::Shader {
    Shader(size_t id);
    ~Shader();
    virtual void Discard();
    virtual void Prepare();
    virtual size_t GetId();
private:
    struct ShaderAttribs {
        int VertexPosition;
        int VertexNormal;
        int VertexTangent;
        int VertexColor;
        int VertexTexCoords;

        ShaderAttribs(size_t shaderId);
        void Prepare();
    };

    size_t mId;
    ShaderAttribs mAttribs;
};

} // namespace impl
} // namespace picogfx
