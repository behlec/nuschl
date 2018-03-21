#pragma once

// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
// clang-format on

#include <nuschl/s_exp.hpp>

#include <nuschl/unittests/vector_printer.hpp>
#include <nuschl/unittests/is_printable.hpp>

#include <iostream>
#include <type_traits>
#include <vector>

namespace nuschl::testing {

template <typename T> struct string_to_x {
    std::string input;
    T expected;

    static_assert(tmetap::is_printable<T>::value,
                  "Template parameter for string_to_x must be printable");
};
}

template <typename T>
typename std::enable_if<std::is_same<decltype(std::declval<std::ostream &>()
                                              << std::declval<const T &>()),
                                     std::ostream &>::value,
                        std::ostream &>::type
operator<<(std::ostream &os, nuschl::testing::string_to_x<T> const &pe) {
    os << "{ \"" << pe.input << "\", " << pe.expected << "}";
    return os;
}

template <typename T>
std::ostream &
operator<<(std::ostream &os,
           const std::vector<nuschl::testing::string_to_x<T>> &vec) {
    os << '[';
    bool first = true;
    for (auto const &i : vec) {
        if (!first)
            os << ", ";
        os << i;
        first = false;
    }
    os << ']';
    return os;
}

namespace boost::test_tools::tt_detail {

template <typename T> struct print_log_value<nuschl::testing::string_to_x<T>> {
    void operator()(std::ostream &os,
                    nuschl::testing::string_to_x<T> const &vec) {
        ::operator<<(os, vec);
    }
};

template <typename T>
struct print_log_value<std::vector<nuschl::testing::string_to_x<T>>> {
    void operator()(std::ostream &os,
                    std::vector<nuschl::testing::string_to_x<T>> const &vec) {
        ::operator<<(os, vec);
    }
};
}
