#pragma once
#include "base.h"
#include <cassert>

namespace allocators {
// TODO min, max
template <typename Allocator, size_t size, size_t top = 1024> struct Freelist {
    Block allocate(size_t n) noexcept {
        if (n == size && _root) {
            Block blk = {_root, n};
            _root = _root.next;
            --_size;
            return blk;
        }
        return _parent.allocate(n);
    }

    void deallocate(Block& blk) {
        if (_size > top || blk.size != size)
            return _parent.deallocate(blk);
        assert(_size < top);
        auto ptr = reinterpret_cast<Node*>(blk.ptr);
        ptr.next = _root;
        _root = ptr;
        ++_size;
    }

    bool owns(const Block& blk) {
        return blk.size == size || _parent.owns(blk);
    }

    bool operator==(const Freelist&) { return false; }
    bool operator!=(const Freelist& other) { return !(*this == other); }

private:
    Allocator _parent;
    size_t _size = top;
    struct Node {
        Node* next;
    } _root;
};
}
