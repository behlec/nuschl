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

struct parse_example {
    std::string input;
    const s_exp *expected;
};
}

std::ostream &operator<<(std::ostream &os,
                         nuschl::testing::parse_example const &pe);

std::ostream &
operator<<(std::ostream &os,
           const std::vector<nuschl::testing::parse_example> &vec);

namespace boost::test_tools::tt_detail {

template <> struct print_log_value<nuschl::testing::parse_example> {
    void operator()(std::ostream &os,
                    nuschl::testing::parse_example const &vec) {
        ::operator<<(os, vec);
    }
};

template <>
struct print_log_value<std::vector<nuschl::testing::parse_example>> {
    void operator()(std::ostream &os,
                    std::vector<nuschl::testing::parse_example> const &vec) {
        ::operator<<(os, vec);
    }
};
}
