#include "geom.h"
#include "gfxdriver.h"
#include "renderdata.h"

namespace picogfx {

Geom* Geom::Create(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices) {
    return new impl::Geom(vertices, numVertices, indices, numIndices);
}

namespace impl {

Geom::Geom(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices) {
    mNumIndices = numIndices;
    mVertexBuffer = GfxDriver::Get().CreateBuffer();
    mIndexBuffer = GfxDriver::Get().CreateBuffer();
    GfxDriver::Get().SetVertexBufferData(mVertexBuffer, vertices, numVertices);
    GfxDriver::Get().SetIndexBufferData(mIndexBuffer, indices, numIndices);
}

Geom::~Geom() {
    GfxDriver::Get().DiscardBuffer(mVertexBuffer);
    GfxDriver::Get().DiscardBuffer(mIndexBuffer);
}

void Geom::Discard() {
    delete this;
}

void Geom::Render(picogfx::RenderData& renderData, bool culling) const {
    GfxDriver::Get().SetCulling(culling);
    GfxDriver::Get().BindBuffers(mVertexBuffer, mIndexBuffer);
    ((RenderData&)renderData).Prepare();
    GfxDriver::Get().RenderTrianglesWithBoundBuffersAndUnbind(mNumIndices);
}

} // namespace impl
} // namespace picogfx
