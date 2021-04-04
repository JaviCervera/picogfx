#include "geom.h"
#include "gfxdriver.h"
#include "shadervars.h"

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

void Geom::Draw(picogfx::ShaderVars& shaderVars, bool culling) const {
    GfxDriver::Get().SetCulling(culling);
    GfxDriver::Get().BindBuffers(mVertexBuffer, mIndexBuffer);
    ((ShaderVars&)shaderVars).Prepare();
    GfxDriver::Get().DrawTrianglesWithBoundBuffersAndUnbind(mNumIndices);
}

} // namespace impl
} // namespace picogfx
