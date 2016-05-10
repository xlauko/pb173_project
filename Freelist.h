#pragma once

#include "base.h"
#include "dynamic_size.h"
#include "type_classes.h"
#include <cassert>

namespace allocators {
template <typename Allocator, size_t min, size_t max, size_t batch_size,
          size_t capacity = 1024>
struct Freelist : Eq, DynamicBlkSize<min, max> {

    Freelist() {}

    Freelist(const Freelist&) = delete;
    Freelist(Freelist&&) = default;
    Freelist& operator=(const Freelist&) = delete;
    Freelist& operator=(Freelist&&) = default;

    ~Freelist() {
        while (_root) {
            Block blk = pop();
            _parent.deallocate(blk);
        }
    }

    Block allocate(size_t n) noexcept {
        if (min_size() <= n && n <= max_size()) {
            if (_root) {
                return pop();
            } else {
                return _parent.allocate(block_size);
            }
        }
        return _parent.allocate(n);
    }

    void deallocate(Block& blk) {
        assert(owns(blk));
        if (_size > capacity || blk.size != block_size)
            return _parent.deallocate(blk);
        push(blk);
    }

    bool owns(const Block& blk) const noexcept { return _parent.owns(blk); }

    bool operator==(const Freelist& b) const { return _root == b._root; }

    using DynamicBlkSize<min, max>::min_size;
    using DynamicBlkSize<min, max>::max_size;
    using DynamicBlkSize<min, max>::block_size;

private:
    void push(Block& blk) {
        if (_size < capacity) {
            Node* ptr = reinterpret_cast<Node*>(blk.ptr);
            ptr->next = _root;
            _root = ptr;
            ++_size;
        }
    }

    Block pop() {
        assert(_root);
        void* ptr = _root;
        _root = _root->next;
        --_size;
        return {ptr, block_size};
    }

    Allocator _parent;
    size_t _size = 0;
    struct Node {
        Node* next;
    }* _root = nullptr;
};
}
