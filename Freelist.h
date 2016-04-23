#pragma once
#include "base.h"
#include <cassert>

namespace allocators {
template <typename Allocator, size_t min, size_t max, size_t batch_size,
          size_t capacity = 1024>
struct Freelist {

    ~Freelist() {
        while (_root) {
            _parent.deallocate(pop());
        }
    }

    Block allocate(size_t n) noexcept {
        if (min <= n && n <= max) {
            if (_root) {
                return pop();
            }

            size_t block_size = max;

            auto batch = _parent.allocate(block_size * batch_size);

            if (batch) {
                for (size_t i = 1; i < batch_size; ++i) {
                    Block blk = {batch + block_size * i, block_size};
                    push(blk);
                }
                Block blk = {batch, block_size};
                return blk;
            }
        }
        return _parent.allocate(n);
    }

    void deallocate(Block& blk) {
        assert(owns(blk));
        if (_size > capacity)
            return _parent.deallocate(blk);
        assert(_size < capacity);
        push(blk);
    }

    bool owns(const Block& blk) {
        return blk.ptr && min <= blk.size && blk.size <= max;
    }

    bool operator==(const Freelist&) { return false; }
    bool operator!=(const Freelist& other) { return !(*this == other); }

private:
    void push(Block& blk) {
        if (_size < capacity) {
            blk.reset();
            auto ptr = reinterpret_cast<Node*>(blk.ptr);
            ptr.next = _root;
            _root = ptr;
            ++_size;
        }
    }

    Block pop() {
        assert(_root);
        Block blk = {_root, max};
        _root = _root.next;
        --_size;
        return blk;
    }

    Allocator _parent;
    size_t _size = 0;
    struct Node {
        Node* next;
    } _root;
};
}
