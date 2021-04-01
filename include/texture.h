#pragma once

namespace picogfx {

enum TextureFilter {
    NONE,
    LINEAR,
    BILINEAR,
    TRILINEAR
};

struct Texture {
    static Texture* Create(size_t width, size_t height, TextureFilter filter, bool isDepth = false);
    virtual void Discard() = 0;
    virtual void SetPixels() = 0;
    virtual size_t GetWidth() = 0;
    virtual size_t GetHeight() = 0;
    virtual TextureFilter GetFilter() = 0;
};

} // namespace picogfx
