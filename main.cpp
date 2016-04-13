//
// Created by kejsty on 12.4.16.
//


#include <assert.h>
#include "NullAllocator.h"
#include "StackAllocator.h"
#include "Mallocator.h"
#include "FallbackAllocator.h"

void NullAllocatorTest() {
    allocators::NullAllocator nullAllocator;
    Block block = nullAllocator.allocate(40);
    assert(!block);
    assert(block.ptr == nullptr);
    assert(block.size == 0);
}

void StackAllocatorTest() {
    allocators::StackAllocator<1024> allocator;
    Block block1 = allocator.allocate(128);
    Block block2 = allocator.allocate(256);
    assert(block1.ptr != nullptr);
    assert(block2.ptr != nullptr);
    assert(block1.size == 128);
    assert(block2.size == 256);
    assert(block1 != block2);
    assert(allocator.owns(block2));
    allocator.deallocate(block2);
    assert(block2.size == 0);
    assert(allocator.owns(block1));
    allocator.deallocate(block1);
    Block block3 = allocator.allocate(1024);
    assert(block3.size == 1024);
    Block block4 = allocator.allocate(100);
    assert(!block4);
}

void MallocatorTest() {
    allocators::Mallocator allocator;
    Block block1 = allocator.allocate(128);
    Block block2 = allocator.allocate(256);
    assert(block1.ptr != nullptr);
    assert(block2.ptr != nullptr);
    assert(block1.size == 128);
    assert(block2.size == 256);
    assert(block1 != block2);
    assert(allocator.owns(block2));
    assert(allocator.owns(block1));
    allocator.deallocate(block1);
    allocator.deallocate(block2);
    assert(block1.size == 0);

}

void FallBackAllocatorTest() {
    allocators::FallbackAllocator<allocators::StackAllocator<256>, allocators::Mallocator> allocator;
    Block block1 = allocator.allocate(250);
    Block block2 = allocator.allocate(250);
    assert(block1.size == 250);
    assert(block2.size == 250);
    assert(allocator.owns(block1));
    assert(allocator.owns(block2));
    allocator.deallocate(block2);
    allocator.deallocate(block1);
}

int main(){
    NullAllocatorTest();
    StackAllocatorTest();
    MallocatorTest();
    FallBackAllocatorTest();
}

