#pragma once

#include <iostream>
#include <type_traits>

namespace nuschl::tmetap {

struct no {
    char a, b;
};
static_assert(sizeof(no) >= 2, "The world is falling down!");
template <typename T> struct is_printable {
    template <typename U>
    static char test(char (*)[sizeof(decltype(std::declval<std::ostream &>()
                                              << std::declval<const U &>()))]);
    template <typename U> static no test(...);

    using type = std::integral_constant<bool, sizeof(test<T>(0)) % 2>;
    constexpr static bool value = type::value;
};
}
