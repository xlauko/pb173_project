---
title: "Allocators"
author: Henruch Lauko
---

## Motivation

-  
-

## Block ?

## C++ Allocators API Requirements

- Block allocate(size_t)
- void deallocate(Block)
- bool owns(Block)

## Allocators

- NullAllocator
- Mallocator
- StackAllocator

- FallbackAllocator
- Freelist
- Segregator
- AffixAllocator
- StatisticCollector
- BitmappedBlock

## FallbackAllocator

- Owns- MDFINAE

## Freelist

- TODO problems

## Segregator

## AffixAllocator

## BitmappedBlock

## Modularity - composability

- composition of allocators, specialized by block sizes
- arrays, lists, trees of allocators

## Example


## Benchmarks
