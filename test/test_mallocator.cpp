#include "Mallocator.h"
#include "catch.hpp"
#include "test_common.h"

using namespace allocators;

TEST_CASE("Mallocator Test") {
    Mallocator allocator;
    Block block1 = allocator.allocate(128);
    Block block2 = allocator.allocate(256);
    test_block(block1, 128);
    test_block(block2, 256);
    REQUIRE(block1 != block2);
    allocator.deallocate(block1);
    allocator.deallocate(block2);
    test_null(block1);
    test_null(block2);
}
/*
TEST_CASE("Mallocator : move constructor, move operator="){
    Mallocator  fst;
    Block block = fst.allocate(10);
    test_block(block,10);
    REQUIRE(fst.owns(block));
    Mallocator  snd (std::move(fst));
    test_block(block,10);
    REQUIRE(snd.owns(block));
//    snd = std::move(snd);
//    test_block(block,10);
//    REQUIRE(snd.owns(block)); TODO:should work?
    fst = Mallocator();
    snd.deallocate(block);
    test_null(block);
    block = fst.allocate(10);
    test_block(block,10);
    snd = std::move(fst);
    test_block(block,10);
    REQUIRE(snd.owns(block));
}
*/
