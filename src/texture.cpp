#include "gfxdriver.h"
#include "texture.h"

namespace picogfx {

Texture* Texture::Create(size_t width, size_t height, TextureFilter filter, bool isDepth) {
    return new impl::Texture(width, height, filter, isDepth);
}

namespace impl {

Texture::Texture(size_t width, size_t height, TextureFilter filter, bool isDepth) : mWidth(width), mHeight(height), mFilter(filter) {
    mId = GfxDriver::Get().CreateTexture(width, height, isDepth);
}

Texture::~Texture() {
    GfxDriver::Get().DiscardTexture(mId);
}

void Texture::Discard() {
    delete this;
}

void Texture::SetPixels(const unsigned char* pixels) {
    GfxDriver::Get().SetTexturePixels(mId, pixels, mWidth, mHeight, mFilter);
}

size_t Texture::GetWidth() const {
    return mWidth;
}

size_t Texture::GetHeight() const {
    return mHeight;
}

TextureFilter Texture::GetFilter() const {
    return mFilter;
}

size_t Texture::GetId() const {
    return mId;
}

} // namespace impl
} // namespace picogfx
