//
// Created by kejsty on 12.4.16.
//

#ifndef PB173_PROJECT_BASE_H
#define PB173_PROJECT_BASE_H

#include <cstddef>
#include <iostream>

struct Block {

    size_t _size;
    void *_ptr;

    Block() : _size (0), _ptr(nullptr){}
    Block(size_t size, void *ptr) : _size(size), _ptr( ptr) {}


    Block(const Block &)= default;
    Block(Block && other) {
            *this = std::move(other);
    }

    Block&operator=(const Block & other) = default;
    Block&operator=(Block && other)  {
        std::swap(*this, other);
        return *this;

    }

    ~Block() = default;

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
        return _size != 0;
    }

};

#endif //PB173_PROJECT_BASE_H
