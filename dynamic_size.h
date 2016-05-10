#pragma once

#include <limits>
#include <cstddef>
#include <cassert>

namespace allocators {

enum dynamic_options : size_t {
    Undefined = std::numeric_limits<size_t>::max(),
    Determined = std::numeric_limits<size_t>::max() - 1
};

template <size_t size, size_t dynamic_enable> struct dynamic_size_t {
    size_t value() const noexcept { return size; }
};

template <size_t dynamic_enable>
struct dynamic_size_t<dynamic_enable, dynamic_enable> {
    dynamic_size_t() noexcept : _value(Undefined) {}

    size_t value() const noexcept { return _value; }

    void value(size_t value) noexcept { _value = value; }

private:
    size_t _value;
};

template <size_t min, size_t max> struct DynamicBlkSize {
    static_assert(
            min <= max,
            "Minimal size must be smaller or equal to maximal block size.");

    static const size_t block_size = max;

    allocators::dynamic_size_t<
            (min == dynamic_options::Determined
                     ? allocators::dynamic_options::Determined
                     : min),
            dynamic_options::Determined> _min_size;
    allocators::dynamic_size_t<
            (max == dynamic_options::Determined
                     ? allocators::dynamic_options::Determined
                     : max),
            dynamic_options::Determined> _max_size;

    void set_bounds(size_t min_bound, size_t max_bound) {
        assert(_min_size.value() == dynamic_options::Undefined);
        assert(_max_size.value() == dynamic_options::Undefined);

        assert(min_bound <= max_bound);
        _min_size.value(min_bound);
        _max_size.value(max_bound);
    }

    // todo specializace pre undefined
    size_t min_size() const noexcept { return _min_size.value(); }

    size_t max_size() const noexcept { return _max_size.value(); }
};

using UndefinedBlkSize = DynamicBlkSize<Undefined, Undefined>;
}:
