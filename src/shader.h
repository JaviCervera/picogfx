#pragma once

#include "../include/shader.h"

namespace picogfx {
namespace impl {

struct Shader : public picogfx::Shader {
    Shader(size_t id);
    ~Shader();
    virtual void Discard();
    void Prepare() const;
    size_t GetId() const;
private:
    struct ShaderAttribs {
        int VertexPosition;
        int VertexNormal;
        int VertexTangent;
        int VertexColor;
        int VertexTexCoords;

        ShaderAttribs(size_t shaderId);
        void Prepare(size_t shaderId) const;
    };

    size_t mId;
    ShaderAttribs mAttribs;
};

} // namespace impl
} // namespace picogfx
