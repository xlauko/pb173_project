#pragma once

#include "traits.h"
#include <memory>

namespace allocators {
struct Block : Eq {
    void* ptr;
    size_t size;

    Block() noexcept : Block(nullptr, 0) {}
    Block(void* ptr, size_t size) noexcept : ptr(ptr), size(size) {}
    Block(Block&& b) noexcept : Block() { swap(*this, b); }
    Block(Block const&) = default;
    ~Block() = default;

    Block& operator=(Block b) noexcept {
        swap(*this, b);
        return *this;
    }

    void reset() noexcept {
        ptr = nullptr;
        size = 0;
    }

    operator bool() const noexcept { return static_cast<bool>(ptr); }

    bool operator==(const Block& other) const noexcept {
        return other.ptr == ptr && size == other.size;
    }

    friend void swap(Block& a, Block& b) noexcept {
        using std::swap;
        swap(a.ptr, b.ptr);
        swap(a.size, b.size);
    }
};
} // namespace allocators
