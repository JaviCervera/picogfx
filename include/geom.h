#pragma once

#include <stddef.h>
#include "vertex.h"

namespace picogfx {

struct RenderData;

enum BlendMode {
    SOLID,
    ALPHA,
    ADD,
    MULTIPLY
};

enum RenderMode {
    POINTS,
    LINES,
    TRIANGLES,
    TRIANGLE_STRIP,
    TRIANGLE_FAN
};

struct Geom {
    static Geom* Create(RenderMode mode, const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices);
    virtual void Discard() = 0;
    virtual void Render(RenderData& renderData, BlendMode blend, bool culling = true, bool depthTest = true, bool depthWrite = true) const = 0;
};

} // namespace picogfx
