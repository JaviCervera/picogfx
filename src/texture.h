#pragma once

#include "../include/texture.h"

namespace picogfx {
namespace impl {

struct Texture : public picogfx::Texture {
    Texture(size_t width, size_t height, TextureFilter filter, bool isDepth);
    ~Texture();
    virtual void Discard();
    virtual void SetPixels(const unsigned char* pixels);
    virtual size_t GetWidth() const;
    virtual size_t GetHeight() const;
    virtual TextureFilter GetFilter() const;
    size_t GetId() const;
private:
    size_t mId;
    size_t mWidth;
    size_t mHeight;
    TextureFilter mFilter;
};

} // namespace impl
} // namespace picogfx
