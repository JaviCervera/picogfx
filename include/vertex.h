#pragma once

namespace picogfx {

struct Vertex {
    float Position[3];
    float Normal[3];
    float Tangent[3];
    int Color;
    float TexCoords[2];

    Vertex(float x, float y, float z, int color, float u = 0, float v = 0) {
        Position[0] = x;
        Position[1] = y;
        Position[2] = z;
        Normal[0] = 0;
        Normal[1] = 0;
        Normal[2] = -1;
        Tangent[0] = 1;
        Tangent[1] = 0;
        Tangent[2] = 0;
        Color = color;
        TexCoords[0] = u;
        TexCoords[1] = v;
    }

    Vertex(float x, float y, float z, float nx, float ny, float nz, float tx, float ty, float tz, int color, float u, float v) {
        Position[0] = x;
        Position[1] = y;
        Position[2] = z;
        Normal[0] = nx;
        Normal[1] = ny;
        Normal[2] = nz;
        Tangent[0] = tx;
        Tangent[1] = ty;
        Tangent[2] = tz;
        Color = color;
        TexCoords[0] = u;
        TexCoords[1] = v;
    }
};

} // namespace picogfx
