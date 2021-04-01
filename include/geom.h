#pragma once

namespace picogfx {

struct GeomSettings;
struct GlobalSettings;

struct Geom {
    static Geom* Create(const Vertex* vertices, size_t numVertices, const unsigned short* indices, size_t numIndices);
    virtual void Discard() = 0;
    virtual void Draw(const GlobalSettings& globalSettings, const float* transform, const GeomSettings& geomSettings) = 0;
};

} // namespace picogfx
