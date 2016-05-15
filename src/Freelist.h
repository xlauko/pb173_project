#pragma once

#include "Block.h"
#include <cassert>

namespace allocators {
template <class Allocator, size_t min, size_t max, size_t capacity = 1024>
struct Freelist : Eq {

    Freelist() = default;
    Freelist(const Freelist&) = delete;
    Freelist(Freelist&& other)
        : _parent(std::move(other._parent))
        , _root(other._root)
        , _size(other._size) {
        other._root = nullptr;
        other._size = 0;
    }

    Freelist& operator=(const Freelist&) = delete;
    Freelist& operator=(Freelist&&){
            // TODO Jirka
    }

    ~Freelist() {
        while (_root) {
            auto b = pop();
            _parent.deallocate(b);
        }
    }

    bool owns(Block const& b) const noexcept {
        return is_inside_bounds(b.size) || _parent.owns(b);
    }

    Block allocate(size_t size) {
        if (is_inside_bounds(size)) {
            if (_root)
                return pop();
            else
                return _parent.allocate(max);
        }
        return _parent.allocate(size);
    }

    void deallocate(Block& b) noexcept {
        assert(owns(b));
        if (_size > capacity || !is_inside_bounds(b.size))
            return _parent.deallocate(b);
        push(b);
        b.reset();
    }

    bool operator==(Freelist const& b) const noexcept {
        return _parent == b._parent;
    }

private:
    struct Node {
        Node* next = nullptr;
    };

    Allocator _parent;
    Node* _root = nullptr;
    size_t _size = 0;

    bool is_inside_bounds(size_t size) const noexcept {
        return min <= size && size <= max;
    }

    void push(Block b) noexcept {
        auto p = static_cast<Node*>(b.ptr);
        p->next = _root;
        _root = p;
        ++_size;
    }

    Block pop() noexcept {
        assert(_root);
        auto p = static_cast<void*>(_root);
        _root = _root->next;
        --_size;
        return {p, max};
    }
};
}
