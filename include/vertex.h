#pragma once

namespace picogfx {

struct Vertex {
    float Position[3];
    float Normal[3];
    float Tangent[3];
    int Color;
    float TexCoords[2];
};

} // namespace picogfx
