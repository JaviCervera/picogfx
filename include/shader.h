#pragma once

namespace picogfx {

struct Shader {
    static Shader* Create(const char* vertex, const char* fragment, char* outError, size_t outSize);
    virtual void Discard() = 0;
};

} // namespace picogfx
