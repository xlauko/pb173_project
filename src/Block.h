#pragma once

#include "traits.h"
#include <memory>

namespace allocators {
struct Block : Eq {
    void* ptr;
    size_t size;

    Block() : Block(nullptr, 0) {}
    Block(void* ptr, size_t size) : ptr(ptr), size(size) {}
    Block(Block&& b) : Block() { swap(*this, b); }
    Block(Block const&) = default;
    ~Block() = default;

    Block& operator=(Block b) noexcept {
        swap(*this, b);
        return *this;
    }

    void reset() {
        ptr = nullptr;
        size = 0;
    }

    operator bool() const { return ptr; }

    bool operator==(const Block& other) const {
        return other.ptr == ptr && size == other.size;
    }

    friend void swap(Block& a, Block& b) noexcept {
        using std::swap;
        swap(a.ptr, b.ptr);
        swap(a.size, b.size);
    }
};
} // namespace allocators
