#pragma once

#include <stddef.h>
#include "vertex.h"

namespace picogfx {

struct ShaderVars;

struct Geom {
    static Geom* Create(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices);
    virtual void Discard() = 0;
    virtual void Draw(ShaderVars& shaderVars, bool culling) const = 0;
};

} // namespace picogfx
