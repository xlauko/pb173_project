//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_BASE_H
#define PB173_PROJECT_BASE_H

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

#endif // PB173_PROJECT_BASE_H
