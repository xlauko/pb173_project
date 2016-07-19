#pragma once

#include "Block.h"
#include <algorithm>
#include <limits>

namespace allocators {
namespace detail {
    inline unsigned count_leading_zeros(uint32_t x) {
#ifdef __GNUC__
        return static_cast<unsigned>(__builtin_clz(x));
#elif _MSC_VER
        return static_cast<unsigned>(__lzcnt(x));
#else
#warning "CAUTION! Unsupported compiler! Slow trivial implmentation is used"
        if (x == 0)
            return 32;
        unsigned n = 1;
        if ((x & 0xffff0000) == 0) {
            x <<= 16;
            n += 16;
        }
        if ((x & 0xff000000) == 0) {
            x <<= 8;
            n += 8;
        }
        if ((x & 0xf0000000) == 0) {
            x <<= 4;
            n += 4;
        }
        if ((x & 0xc0000000) == 0) {
            x <<= 2;
            n += 2;
        }
        return (x & 0x80000000) ? n - 1 : n;
#endif
    }

    inline unsigned count_leading_zeros(uint64_t x) {
#ifdef __GNUC__
        return static_cast<unsigned>(__builtin_clzl(x));
#elif _MSC_VER
        return static_cast<unsigned>(__lzcnt64(x));
#else
#warning "CAUTION! Unsupported compiler! Slow trivial implmentation is used"
        if (x == 0)
            return 64;
        unsigned n = 1;
        if ((x & 0xffffffff00000000) == 0) {
            x <<= 32;
            n += 32;
        }
        if ((x & 0xffff000000000000) == 0) {
            x <<= 16;
            n += 16;
        }
        if ((x & 0xff00000000000000) == 0) {
            x <<= 8;
            n += 8;
        }
        if ((x & 0xf000000000000000) == 0) {
            x <<= 4;
            n += 4;
        }
        if ((x & 0xc000000000000000) == 0) {
            x <<= 2;
            n += 2;
        }
        return (x & 0x8000000000000000) ? n - 1 : n;
#endif
    }

    template <class T> constexpr T ceil_div(T a, T b) noexcept {
        return 1 + ((a - 1) / b);
    }
} // namespace detail
} // namespace allocators

namespace allocators {
template <class Allocator> struct BitSet : Eq {
    struct reference {
        reference(std::uintmax_t& ref, std::uintmax_t mask)
            : _ref(ref), _mask(mask) {}

        operator bool() const noexcept { return _ref & _mask; }

        void flip() noexcept { _ref ^= _mask; }

    private:
        std::uintmax_t& _ref;
        std::uintmax_t _mask;
    };

public:
    BitSet(std::size_t size, Allocator& alloc)
        : _alloc(alloc)
        , _size(size)
        , _data(_alloc.allocate(size_in_blocks() * sizeof(std::uintmax_t))) {
        if (!_data.ptr)
            throw std::bad_alloc();
        set_false();
    }

    BitSet(BitSet const& b) : BitSet(b._size, b._alloc) {
        std::copy_n(data_as_blocks(), size_in_blocks(), b.data_as_blocks());
    }

    BitSet(BitSet&& b) = default;

    ~BitSet() { _alloc.deallocate(_data); }

    BitSet& operator=(BitSet b) {
        using std::swap;
        swap(_alloc, b._alloc);
        swap(_size, b._size);
        swap(_data, b._data);
        return *this;
    }

public:
    std::size_t size() const noexcept { return _size; }

    bool operator==(BitSet const& b) const noexcept {
        return _size == b._size &&
               std::equal(data_as_blocks(), data_as_blocks() + size_in_blocks(),
                          b.data_as_blocks());
    }

    reference operator[](size_t i) noexcept {
        return {data_as_blocks()[i / bits_in_block],
                std::uintmax_t(1u)
                        << (bits_in_block - 1 - (i % bits_in_block))};
    }

    bool operator[](size_t i) const noexcept {
        return data_as_blocks()[i / bits_in_block] &
               (std::uintmax_t(1u)
                << (bits_in_block - 1 - (i % bits_in_block)));
    }

    std::size_t find_first() const noexcept {
        size_t i = 0;

        auto beg = data_as_blocks();
        auto end = data_as_blocks() + size_in_blocks();

        while (beg != end && *beg == 0) {
            ++beg;
            ++i;
        }
        if (beg == end)
            return _size;
        return i * bits_in_block + detail::count_leading_zeros(*beg);
    }

    void set_false() {
        std::fill_n(data_as_blocks(), size_in_blocks(), std::uintmax_t(0u));
    }

    void set_true() {
        std::fill_n(data_as_blocks(), size_in_blocks(), ~std::uintmax_t(0u));
    }

private:
    std::uintmax_t const* data_as_blocks() const noexcept {
        return static_cast<std::uintmax_t*>(_data.ptr);
    }

    std::uintmax_t* data_as_blocks() noexcept {
        return static_cast<std::uintmax_t*>(_data.ptr);
    }

    std::size_t size_in_blocks() const noexcept {
        return detail::ceil_div(_size, bits_in_block);
    }

private:
    Allocator& _alloc;
    std::size_t _size;
    Block _data;

    static constexpr std::size_t bits_in_block =
            std::numeric_limits<std::uintmax_t>::digits;
};
} // namespace allocators
