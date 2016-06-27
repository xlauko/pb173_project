#pragma once

#include <type_traits>

namespace std {
template <class T, class U>
constexpr bool is_base_of_v = std::is_base_of<T, U>::value;
} // namespace std

struct Eq {};
struct Ord : Eq {};

template <class T>
auto operator!=(const T& a, const T& b)
        -> std::enable_if_t<std::is_base_of_v<Eq, T>, bool> {
    return !(a == b);
}

template <class T>
auto operator>(const T& a, const T& b)
        -> std::enable_if_t<std::is_base_of_v<Ord, T>, bool> {
    return b < a;
}

template <class T>
auto operator<=(const T& a, const T& b)
        -> std::enable_if_t<std::is_base_of_v<Ord, T>, bool> {
    return !(b < a);
}

template <class T>
auto operator>=(const T& a, const T& b)
        -> std::enable_if_t<std::is_base_of_v<Ord, T>, bool> {
    return !(a < b);
}

template <class E> auto to_underlying(E value) -> std::underlying_type_t<E> {
    return static_cast<std::underlying_type_t<E>>(value);
}
