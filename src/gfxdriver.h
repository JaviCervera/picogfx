#pragma once

#include "../include/core.h"
#include "../include/texture.h"

namespace picogfx {

struct Vertex;

namespace impl {

struct GfxDriver {
    static GfxDriver& Get();

    virtual bool Init(Core::LoadProc proc) = 0;
    virtual void Prepare(int viewportX, int viewportY, int viewportWidth, int viewportHeight, int color) = 0;

    virtual size_t CreateBuffer() = 0;
    virtual void DiscardBuffer(size_t buffer) = 0;
    virtual void SetVertexBufferData(size_t buffer, const Vertex* vertices, size_t numVertices) = 0;
    virtual void SetIndexBufferData(size_t buffer, const unsigned short* indices, size_t numIndices) = 0;
    virtual void BindBuffers(size_t vertexBuffer, size_t indexBuffer) = 0;
    virtual void DrawTrianglesWithBoundBuffersAndUnbind(size_t numIndices) = 0;

    virtual size_t CreateShader(const char* vertex, const char* fragment, char* outError, size_t outSize) = 0;
    virtual void DiscardShader(size_t shader) = 0;
    virtual void BindShader(size_t shader) = 0;
    virtual int GetShaderVertexVarLocation(size_t shader, const char* name) = 0;
    virtual void SetVertexVar(size_t shader, int location, size_t size, bool normalize, size_t offset) = 0;
    virtual int GetShaderVarLocation(size_t shader, const char* name) = 0;
    virtual void SetShaderInt(size_t shader, int location, int val) = 0;
    virtual void SetShaderFloat(size_t shader, int location, float val) = 0;
    virtual void SetShaderVec2(size_t shader, int location, float* val) = 0;
    virtual void SetShaderVec3(size_t shader, int location, float* val) = 0;
    virtual void SetShaderVec4(size_t shader, int location, float* val) = 0;
    virtual void SetShaderMat4(size_t shader, int location, float* val) = 0;

    virtual size_t CreateTexture(size_t width, size_t height, bool isDepth) = 0;
    virtual void DiscardTexture(size_t tex) = 0;
    virtual void BindTexture(size_t tex, size_t unit) = 0;
    virtual void SetTexturePixels(size_t tex, const void* pixels, size_t width, size_t height, TextureFilter filter) = 0;
};

} // namespace impl
} // namespace picogfx
