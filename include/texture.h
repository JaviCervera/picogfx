#pragma once

#include <stddef.h>

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
    virtual void SetPixels(const unsigned char* pixels) = 0;
    virtual size_t GetWidth() const = 0;
    virtual size_t GetHeight() const = 0;
    virtual TextureFilter GetFilter() const = 0;
};

} // namespace picogfx
