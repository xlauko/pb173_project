#include "Block.h"
#include "BitSet.h"

namespace allocators {
template <class Allocator, size_t block_size> struct BitmappedBlock : Eq {
    BitmappedBlock(size_t size)
        : _data(_allocator.allocate(size * block_size)), _freelist(size) {
        if (!_data)
            throw std::bad_alloc();
    }
    BitmappedBlock(BitmappedBlock&&) = default;
    BitmappedBlock(BitmappedBlock const&) = delete;
    ~BitmappedBlock() { _allocator.dealocate(_data); }

    BitmappedBlock& operator=(const BitmappedBlock&) = delete;
    BitmappedBlock& operator=(BitmappedBlock&&) = default;

    Block allocate(size_t size) noexcept {
        if (size != block_size)
            return {nullptr, 0};

        size_t i = _freelist.find_first();
        if (i >= _freelist.size())
            return {nullptr, 0};

        _freelist[i].flip();
        return {_data.ptr + (i * block_size), block_size};
    }

    void dealocate(Block& blk) noexcept {
        assert(owns(blk));
        size_t i = static_cast<size_t>(blk.ptr - _data.ptr);
        _freelist[i / block_size].flip();
        blk = {nullptr, 0};
    }

    bool owns(Block const& blk) const noexcept {
        return _data.ptr <= blk.ptr && blk.ptr < _data.ptr + _data.size;
    }

    bool operator==(BitmappedBlock const& b) const noexcept {
        return _data == b._data;
    }

private:
    Allocator _allocator;
    Block _data;
    BitSet<Allocator> _freelist;
};
} // namespace allocators
