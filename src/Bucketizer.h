#pragma once
/*
#include "Block.h"
#include <cassert>

namespace {
template <typename Allocator, size_t min, size_t max, size_t step>
struct Bucketizer : Eq {
    static_assert(min < max, "Minimal size must be smaller than maximal.");
    static_assert((max - min + 1) % step == 0, "Step size is incorrect.");

    static constexpr size_t number_of_buckets = ((max - min + 1) / step);

    Bucketizer() {
        for (size_t i = 0; i < number_of_buckets; ++i) {
            _buckets[i].set_bounds(i * step + min, (i + 1) * step + min - 1);
        }
    }

    Bucketizer(const Bucketizer&) = delete;
    Bucketizer(Bucketizer&&) = default;
    Bucketizer& operator=(const Bucketizer&) = delete;
    Bucketizer& operator=(Bucketizer&&) = default;

    Block allocate(size_t n) {
        if (bucket_index(n) < number_of_buckets)
            return get_bucket_allocator(n)->allocate(n);
        return {};
    }

    void deallocate(Block& blk) noexcept {
        if (!blk)
            return;
        assert(owns(blk));
        get_bucket_allocator(blk.size)->deallocate(blk);
    }

    bool owns(const Block& blk) const noexcept {
        return blk && bucket_index(blk.size) < number_of_buckets &&
               _buckets[bucket_index(blk.size)].owns(blk);
    }

    bool operator==(const Bucketizer& b) const {
        return _buckets == b._buckets;
    }

    Allocator _buckets[number_of_buckets];

private:
    size_t bucket_index(size_t n) const noexcept { return (n / step) - 1; }

    Allocator* get_bucket_allocator(size_t n) noexcept {
        assert(min <= n && n < max);
        return &_buckets[bucket_index(n)];
    }
};

template <class Allocator, size_t min, size_t max, size_t step>
const size_t Bucketizer<Allocator, min, max, step>::number_of_buckets;
}
*/

#include "Block.h"

namespace allocators {
namespace detail {
    /*    template <typename T> struct id { using type = T; };

    template <size_t min, size_t max, size_t step>
    const size_t number_of_buckets = (max - min + 1) / step;

    template <size_t N, typename... B> struct Choose;

    template <size_t N, typename H, typename... T>
    struct Choose<N, H, T...> : Choose<N - 1, T...> {};

    template <typename H, typename... T> struct Choose<0, H, T...> : id<H> {};

    template <size_t N, typename... T>
    using choose = typename Choose<N, T...>::type;

    template <size_t N, typename Alloc> struct Bucket {
        Alloc _allocator;

    public:
        Alloc& get() { return _allocator; }
        const Alloc& get() const { return _allocator; }
    };

    template <typename N, class... Bs> struct BucketsImpl;

    template <size_t... N, class... Bs>
    struct BucketsImpl<std::index_sequence<N...>, Bs...> : Bucket<N, Bs>... {};

    template <template <size_t, size_t> class Alloc, size_t min, size_t max,
              size_t step, Bucket<Alloc<min, max>>>
    struct make_buckets {};

    template <template <size_t, size_t> class Alloc, size_t min, size_t max,
              size_t step>
    struct Buckets
            : BucketsImpl<
                      std::index_sequence<number_of_buckets<min, max, step>>,
                      make_buckets<Alloc, min, max, step>> {};
    */ /*template
                                                             <typename Alloc,
                                                             size_t min, size_t
                                                             max, size_t step,
                                                             class...
                                                             Ts>
                                                             struct make_buckets
                                                             :
                                                             make_buckets<Alloc,
                                                             min + step, max,
                                                             step,
                                                                                                Alloc<min, max>, Ts...> {};

                                                             template <typename
                                                             Alloc, size_t min,
                                                             size_t step,
                                                             class... Ts>
                                                             struct
                                                             make_buckets<Alloc,
                                                             min, min, step,
                                                             Ts...> {
                                                                 using type =
                                                             std::tuple<Ts...>;
                                                             };

                                                             template <typename
                                                             Alloc, size_t min,
                                                             size_t max, size_t
                                                             step, class... Ts>
                                                             using
                                                             make_buckets_t =
                                                                     typename
                                                             make_buckets<Alloc,
                                                             min, max, step,
                                                             Ts...>::type;*/
} // namespace detail

template <typename Allocator, size_t min, size_t max, size_t step>
struct Bucketizer : Eq {
    static_assert(min < max, "Minimal size must be smaller than maximal.");
    static_assert((max - min + 1) % step == 0, "Step size is incorrect.");

    static constexpr size_t number_of_buckets = ((max - min + 1) / step);

    bool owns(Block const& b) const noexcept;

    Block allocate(size_t size) {}

    void dealocate(Block& b) noexcept {}

    bool operator==(Bucketizer const& b) const noexcept {
        return true; //_buckets == b._buckets;
    }

private:
    size_t bucket_index(size_t n) const noexcept { return (n / step) - 1; }

    // detail::make_buckets_t<Allocator, min, max, step> _buckets;
};

template <typename Allocator, size_t min, size_t max, size_t step>
const size_t Bucketizer<Allocator, min, max, step>::number_of_buckets;
} // namespace allocators
