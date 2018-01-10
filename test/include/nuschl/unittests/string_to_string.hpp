#pragma once

// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
// clang-format on

#include <nuschl/s_exp.hpp>

#include <nuschl/unittests/vector_printer.hpp>

#include <iostream>
#include <vector>

namespace nuschl::testing {

struct string_to_string {
    std::string input;
    std::string expected;
};
}

std::ostream &operator<<(std::ostream &os,
                         nuschl::testing::string_to_string const &pe);

std::ostream &
operator<<(std::ostream &os,
           const std::vector<nuschl::testing::string_to_string> &vec);

namespace boost::test_tools::tt_detail {

template <> struct print_log_value<nuschl::testing::string_to_string> {
    void operator()(std::ostream &os,
                    nuschl::testing::string_to_string const &vec) {
        ::operator<<(os, vec);
    }
};

template <>
struct print_log_value<std::vector<nuschl::testing::string_to_string>> {
    void operator()(std::ostream &os,
                    std::vector<nuschl::testing::string_to_string> const &vec) {
        ::operator<<(os, vec);
    }
};
}
