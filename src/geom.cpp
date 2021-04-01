#include "geom.h"
#include "gfxdriver.h"

namespace picogfx {

Geom* Geom::Create(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices) {
    return new Geom(vertices, numVertices, indices, numIndices);
}

namespace impl {

Geom::Geom(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices) {
    mNumIndices = numIndices;
    mVertexBuffer = GfxDriver::Get().CreateBuffer();
    mIndicesBuffer = GfxDriver::Get().CreateBuffer();
    GfxDriver::Get().SetVertexBufferData(mVertexBuffer, vertices, numVertices);
    GfxDriver::Get().SetIndexBufferData(mIndexBuffer, indices, numIndices);
}

Geom::~Geom() {
    GfxDriver::Get().DiscardBuffer(mVertexBuffer);
    GfxDriver::Get().DiscardBuffer(mIndexBuffer);
}

void GeomImpl::Discard() {
    delete this;
}

void GeomImpl::Draw(const float* transform, const ShaderVars& shaderVars) {
    GfxDriver::Get().BindBuffers(mVertexBuffer, mIndexBuffer);
    shaderVars.Prepare();
    GfxDriver::Get().DrawTrianglesWithBoundBuffersAndUnbind(mNumIndices);
}

} // namespace impl
} // namespace picogfx
