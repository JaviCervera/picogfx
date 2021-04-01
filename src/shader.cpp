#include "gfxdriver.h"
#include "shader.h"

namespace picogfx {

Shader* Shader::Create(const char* vertex, const char* fragment, char* outError, size_t outSize) {
    size_t id = GfxDriver::Get().CreateShader(vertex, fragment, outError, outSize);
    return (id != 0) ? new Shader(id) : NULL;
}

namespace impl {

Shader::Shader(size_t id) : mId(id), mAttribs(id) {
}

Shader::~Shader() {
    GfxDriver::Get().DiscardShader(mId);
}

virtual void Shader::Discard() {
    delete this;
}

virtual void Shader::Prepare() {
    GfxDriver::Get().BindShader(mShader->GetId());
    mAttribs.Prepare();
}

virtual size_t Shader::GetId() {
    return mId;
}

void Shader::ShaderAttribs::ShaderAttribs(size_t shaderId) {
    VertexPosition = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexPosition");
    VertexNormal = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexNormal");
    VertexTangent = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexTangent");
    VertexColor = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexColor");
    VertexTexCoords = GfxDriver::Get().GetShaderVertexVarLocation(shaderId, "VertexTexCoords");
}

void Shader::ShaderAttribs::Prepare() {
    GfxDriver()::Get().SetVertexVar(GetId(), VertexPosition, 3, false, offsetof(Vertex, Position));
    GfxDriver()::Get().SetVertexVar(GetId(), VertexNormal, 3, false, offsetof(Vertex, Normal));
    GfxDriver()::Get().SetVertexVar(GetId(), VertexTangent, 3, false, offsetof(Vertex, Tangent));
    GfxDriver()::Get().SetVertexVar(GetId(), VertexColor, 4, true, offsetof(Vertex, Color));
    GfxDriver()::Get().SetVertexVar(GetId(), VertexTexCoords, 2, false, offsetof(Vertex, TexCoords));
}

} // namespace impl
} // namespace picogfx
