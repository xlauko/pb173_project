#include <algorithm>
#include <limits>

namespace allocators {
unsigned count_leading_zeros(uint32_t x) {
#ifdef __GNUC__
    return static_cast<unsigned>(__builtin_clz(x));
#elif _MSC_VER
    return static_cast<unsigned>(__lzcnt(x));
#endif
}

unsigned count_leading_zeros(uint64_t x) {
#ifdef __GNUC__
    return static_cast<unsigned>(__builtin_clzl(x));
#elif _MSC_VER
    return static_cast<unsigned>(__lzcnt64(x));
#endif
}
} // namespace allocators

namespace allocators {
template <class Allocator> struct BitSet {
    using block_type = uint64_t;
    using const_reference = bool;

    struct reference {
        reference(block_type& ref, block_type mask) : _ref(ref), _mask(mask) {}

        operator bool() const { return _ref & _mask; }
        void flip() const { _ref ^= _mask; }

    private:
        block_type& _ref;
        block_type _mask;
    };

    BitSet(size_t size);
    BitSet(BitSet&& b) noexcept : _data(b._data), _size(b._size) {
        b._data = nullptr;
        b._size = 0;
    }
    BitSet(BitSet const& b) noexcept : BitSet(b._size) { std::copy_n(b._data, _size, _data); }
    ~BitSet();

    size_t size() const noexcept { return _size; }

    reference operator[](size_t i) noexcept {
        return {_data[i / bits_in_block], 1u << (i % bits_in_block)};
    }
    const_reference operator[](size_t i) const noexcept {
        return _data[i / bits_in_block] & (1u << (i % bits_in_block));
    }

    size_t find_first() const noexcept {
        auto beg = _data;
        auto end = _data + _size;

        while (*beg == 0 && beg != end)
            ++beg;
        return static_cast<size_t>(beg - _data) * bits_in_block + count_leading_zeros(*beg);
    }

private:
    constexpr static auto bits_in_block = std::numeric_limits<block_type>::digits;

    block_type* _data;
    size_t _size;
};
} // namespace allocators
