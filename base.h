//
// Created by kejsty on 12.4.16.
//
#pragma once

#include <cstddef>

struct Block {
    void* ptr;
    size_t size;

    Block() : ptr(nullptr), size(0) {}
    Block(void* ptr, size_t size) : ptr(ptr), size(size) {}

    void reset() {
        ptr = nullptr;
        size = 0;
    }

    bool operator==(const Block& other) { return other.ptr == this->ptr; }
    bool operator!=(const Block& other) { return other.ptr != this->ptr; }

    // to act like ptr
    operator bool() { return ptr; }
};
