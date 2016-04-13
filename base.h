//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_BASE_H
#define PB173_PROJECT_BASE_H

#include <cstddef>

struct Block {

    size_t _size;
    void *_ptr;

    Block() : _size (0), _ptr(nullptr){}
    Block(size_t size, void *ptr) : _size(size), _ptr( ptr) {}

    void reset(){
        _ptr = nullptr;
        _size = 0;
    }

    bool operator== (const Block&other) {
        return other._ptr == this->_ptr;
    }

    bool operator!= (const Block&other) {
        return other._ptr != this->_ptr;
    }

    //to act like ptr
    operator bool(){
        return _ptr;
    }

};

#endif //PB173_PROJECT_BASE_H
