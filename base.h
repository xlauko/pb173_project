#pragma once

#include "type_classes.h"
#include <cstddef>

struct Block : Eq {
    void* ptr;
    size_t size;

    Block() : ptr(nullptr), size(0) {}
    Block(void* ptr, size_t size) : ptr(ptr), size(size) {}

    void reset() {
        ptr = nullptr;
        size = 0;
    }

    operator bool() { return ptr; }

    bool operator==(const Block& other) const { return other.ptr == ptr; }
};
