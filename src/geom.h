#pragma once

#include "../include/geom.h"

namespace picogfx {
namespace impl {

struct Geom : public picogfx::Geom {
    Geom(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices);
    ~Geom();
    virtual void Discard();
    virtual void Draw(ShaderVars& shaderVars) const;
private:
    size_t mVertexBuffer;
    size_t mIndexBuffer;
    size_t mNumIndices;
};

} // namespace impl
} // namespace picogfx
