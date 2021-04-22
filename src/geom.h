#pragma once

#include "../include/geom.h"

namespace picogfx {
namespace impl {

struct Geom : public picogfx::Geom {
    Geom(RenderMode mode, const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices);
    ~Geom();
    virtual void Discard();
    virtual void Render(picogfx::RenderData& renderData, BlendMode blend, bool culling) const;
private:
    RenderMode mMode;
    size_t mVertexBuffer;
    size_t mIndexBuffer;
    size_t mNumIndices;
};

} // namespace impl
} // namespace picogfx
