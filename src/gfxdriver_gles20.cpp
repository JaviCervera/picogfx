#include "../include/texture.h"
#include "../include/vertex.h"
#include "glad_gles20/include/glad/glad.h"
#include "gfxdriver.h"

namespace picogfx {
namespace impl {

struct GfxDriverGLES20 : public GfxDriver {
    virtual size_t CreateBuffer() {
        size_t buffer;
        glGenBuffers(1, &buffer);
        return buffer;
    }

    virtual void DiscardBuffer(size_t buffer) {
        glDeleteBuffers(1, &buffer);
    }

    virtual void SetVertexBufferData(size_t buffer, const Vertex* vertices, size_t numVertices) {
        glBindBuffer(GL_ARRAY_BUFFER, buffer);
        glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(Vertex), vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    virtual void SetIndexBufferData(size_t buffer, const unsigned short* indices, size_t numIndices) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, numIndices * sizeof(unsigned short), indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    virtual void BindBuffers(size_t vertexBuffer, size_t indexBuffer) {
        glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);
    }


    virtual void DrawTrianglesWithBoundBuffersAndUnbind(size_t numIndices) {
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_SHORT, 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    virtual size_t CreateShader(const char* vertex, const char* fragment, char* outError, size_t outSize) {
        GLint retCode;

        // Create vertex shader
        GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vshader, 1, &vertex, NULL);
        glCompileShader(vshader);
        glGetShaderiv(vshader, GL_COMPILE_STATUS, &retCode);
        if (retCode == GL_FALSE) {
            if (outError) glGetShaderInfoLog(vshader, outSize, NULL, outError);
            glDeleteShader(vshader);
            return 0;
        }

        // Create fragment shader
        GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fshader, 1, &fragment, NULL);
        glCompileShader(fshader);
        glGetShaderiv(fshader, GL_COMPILE_STATUS, &retCode);
        if (retCode == GL_FALSE) {
            if (outError) glGetShaderInfoLog(fshader, outSize, NULL, outError);
            glDeleteShader(vshader);
            glDeleteShader(fshader);
            return 0;
        }

        // Create program
        size_t id = glCreateProgram();
        glAttachShader(id, vshader);
        glAttachShader(id, fshader);
        glLinkProgram(id);
        glDeleteShader(vshader);
        glDeleteShader(fshader);
        glGetProgramiv(id, GL_LINK_STATUS, &retCode);
        if (retCode == GL_FALSE) {
            if (outError) glGetProgramInfoLog(id, outSize, NULL, outError);
            glDeleteProgram(id);
            return 0;
        }
        return id;
    }

    virtual void DiscardShader(size_t shader) {
        glDeleteProgram(shader);
    }

    virtual void BindShader(size_t shader) {
        glUseProgram(shader);
    }

    virtual int GetShaderVertexVarLocation(size_t shader, const char* name) {
        return glGetAttribLocation(shader, name);
    }

    virtual void SetVertexVar(size_t shader, int location, size_t size, bool normalize, size_t offset) {
        if (location != -1) {
            glEnableVertexAttribArray(location);
            glVertexAttribPointer(location, size, normalize ? GL_UNSIGNED_BYTE : GL_FLOAT, normalize, sizeof(Vertex), (const void*)offset);
        }
    }

    virtual int GetShaderLocation(size_t shader, const char* name) {
        return glGetUniformLocation(shader, name);
    }

    virtual void SetShaderInt(size_t shader, int location, int val) {
        glUniform1i(location, val);

    }

    virtual void SetShaderFloat(size_t shader, int location, float val) {
        glUniform1f(location, val);
    }

    virtual void SetShaderVec2(size_t shader, int location, float* val) {
        glUniform2fv(location, 1, val);
    }

    virtual void SetShaderVec3(size_t shader, int location, float* val) {
        glUniform3fv(location, 1, val);
    }

    virtual void SetShaderVec4(size_t shader, int location, float* val) {
        glUniform4fv(location, 1, val);
    }

    virtual void SetShaderMat4(size_t shader, int location, float* val) {
        glUniformMatrix4fv(location, 1, false, val);
    }

    virtual size_t CreateTexture(size_t width, size_t height, bool isDepth) {
        size_t id;
        glGenTextures(1, &id);
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, isDepth ? GL_DEPTH_COMPONENT16 : GL_RGBA, width, height, 0, isDepth ? GL_DEPTH_COMPONENT : GL_RGBA, GL_UNSIGNED_BYTE, NULL);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    virtual void DiscardTexture(size_t tex) {
        glDeleteTextures(1, &tex);
    }

    virtual void SetTexturePixels(size_t tex, const void* pixels, size_t width, size_t height, TextureFilter filter) {
        glBindTexture(GL_TEXTURE_2D, id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, MinFilter(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, MagFilter(filter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
        if (filter > TextureFilter::LINEAR) glGenerateMipmap(GL_TEXTURE_2D);
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    static GLint MinFilter(TextureFilter filter) {
        switch (filter) {
        case NONE:
            return GL_NEAREST
        case LINEAR:
            return GL_LINEAR
        case BILINEAR:
            return GL_LINEAR;
        case TRILINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            return GL_LINEAR;
        }
    }

    static GLint MagFilter(TextureFilter filter) {
        return (filter == NONE)
            ? GL_NEAREST
            : GL_LINEAR;
    }
};

static GfxDriver& GfxDriver::Get() {
    static GfxDriverGLES20* instance = NULL;
    if (!instance) instance = new GfxDriverGLES20();
    return *instance;
}

} // namespace impl
} // namespace picogfx
