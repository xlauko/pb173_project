#include "BitSet.h"
#include "Block.h"
#include <cassert>

namespace allocators {
template <class Allocator, size_t block_size> struct BitmappedBlock : Eq {
    BitmappedBlock(size_t size)
        : _data(_allocator.allocate(size * block_size))
        , _freelist(size, _allocator) {
        if (!_data)
            throw std::bad_alloc();
        _freelist.set_true();
    }
    BitmappedBlock(BitmappedBlock&&) = default;
    BitmappedBlock(BitmappedBlock const&) = delete;
    ~BitmappedBlock() { _allocator.deallocate(_data); }

    BitmappedBlock& operator=(const BitmappedBlock&) = delete;
    BitmappedBlock& operator=(BitmappedBlock&&) = default;

    Block allocate(size_t size) noexcept {
        if (size != block_size)
            return {nullptr, 0};

        size_t i = _freelist.find_first();
        if (i >= _freelist.size())
            return {nullptr, 0};

        _freelist[i].flip();
        return {static_cast<block_t*>(_data.ptr) + i, block_size};
    }

    void deallocate(Block& blk) noexcept {
        assert(owns(blk));
        auto i = static_cast<block_t*>(blk.ptr) -
                 static_cast<block_t*>(_data.ptr);

        _freelist[static_cast<size_t>(i)].flip();
        blk = {nullptr, 0};
    }

    bool owns(Block const& blk) const noexcept {
        return _data.ptr <= blk.ptr &&
               blk.ptr < static_cast<void*>(static_cast<block_t*>(_data.ptr) +
                                            _data.size);
    }

    bool operator==(BitmappedBlock const& b) const noexcept {
        return _data == b._data;
    }

private:
    using block_t = std::aligned_storage_t<block_size>;

    Allocator _allocator;
    Block _data;
    BitSet<Allocator> _freelist;
};
} // namespace allocators
