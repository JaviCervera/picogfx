#include "../include/vertex.h"
#include "gfxdriver.h"
#include "shader.h"

namespace picogfx {

Shader* Shader::Create(const char* vertex, const char* fragment, char* outError, size_t outSize) {
    size_t id = impl::GfxDriver::Get().CreateShader(vertex, fragment, outError, outSize);
    return (id != 0) ? new impl::Shader(id) : NULL;
}

namespace impl {

Shader::Shader(size_t id) : mId(id), mAttribs(id) {
}

Shader::~Shader() {
    GfxDriver::Get().DiscardShader(mId);
}

void Shader::Discard() {
    delete this;
}

void Shader::Prepare() const {
    GfxDriver::Get().BindShader(GetId());
    mAttribs.Prepare(GetId());
}

size_t Shader::GetId() const {
    return mId;
}

Shader::ShaderAttribs::ShaderAttribs(size_t shaderId) {
    VertexPosition = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexPosition");
    VertexNormal = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexNormal");
    VertexTangent = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexTangent");
    VertexColor = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexColor");
    VertexTexCoords = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexTexCoords");
}

void Shader::ShaderAttribs::Prepare(size_t shaderId) const {
    GfxDriver::Get().SetVertexVar(shaderId, VertexPosition, 3, false, offsetof(Vertex, Position));
    GfxDriver::Get().SetVertexVar(shaderId, VertexNormal, 3, false, offsetof(Vertex, Normal));
    GfxDriver::Get().SetVertexVar(shaderId, VertexTangent, 3, false, offsetof(Vertex, Tangent));
    GfxDriver::Get().SetVertexVar(shaderId, VertexColor, 4, true, offsetof(Vertex, Color));
    GfxDriver::Get().SetVertexVar(shaderId, VertexTexCoords, 2, false, offsetof(Vertex, TexCoords));
}

} // namespace impl
} // namespace picogfx
