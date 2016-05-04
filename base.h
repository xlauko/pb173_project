#pragma once

#include "type_classes.h"
#include <cstddef>
#include <memory>

struct Block : Eq {
    void* ptr;
    size_t size;

    Block() noexcept : ptr(nullptr), size(0) {}
    Block(void* ptr, size_t size) : ptr(ptr), size(size) {}
    Block(Block&& blk) noexcept { *this = std::move(blk); }
    Block& operator=(Block&& blk) noexcept {
        ptr = blk.ptr;
        size = blk.size;
        blk.reset();
        return *this;
    }

    Block& operator=(const Block& blk) noexcept = default;
    Block(const Block& blk) noexcept = default;

    ~Block() {}

    void reset() {
        ptr = nullptr;
        size = 0;
    }

    operator bool() const { return ptr; }

    bool operator==(const Block& other) const {
        return other.ptr == ptr && size == other.size;
    }
};
