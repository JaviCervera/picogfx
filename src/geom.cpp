#include "geom.h"
#include "gfxdriver.h"
#include "renderdata.h"

namespace picogfx {

Geom* Geom::Create(RenderMode mode, const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices) {
    return new impl::Geom(mode, vertices, numVertices, indices, numIndices);
}

namespace impl {

Geom::Geom(RenderMode mode, const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices) {
    mMode = mode;
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

void Geom::Render(picogfx::RenderData& renderData, picogfx::BlendMode blend, bool culling, bool depthTest, bool depthWrite) const {
    GfxDriver::Get().SetBlendMode(blend);
    GfxDriver::Get().SetCulling(culling);
    GfxDriver::Get().SetDepthTest(depthTest);
    GfxDriver::Get().SetDepthWrite(depthWrite);
    GfxDriver::Get().BindBuffers(mVertexBuffer, mIndexBuffer);
    ((RenderData&)renderData).Prepare();
    switch (mMode) {
    case POINTS:
        GfxDriver::Get().RenderPointsWithBoundBuffersAndUnbind(mNumIndices);
        break;
    case LINES:
        GfxDriver::Get().RenderLinesWithBoundBuffersAndUnbind(mNumIndices);
        break;
    case TRIANGLES:
        GfxDriver::Get().RenderTrianglesWithBoundBuffersAndUnbind(mNumIndices);
        break;
    case TRIANGLE_STRIP:
        GfxDriver::Get().RenderTriangleStripWithBoundBuffersAndUnbind(mNumIndices);
        break;
    case TRIANGLE_FAN:
        GfxDriver::Get().RenderTriangleFanWithBoundBuffersAndUnbind(mNumIndices);
        break;
    }
}

} // namespace impl
} // namespace picogfx
