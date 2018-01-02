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

struct print_test {
    const s_exp *input;
    std::string expected;
};
}

std::ostream &operator<<(std::ostream &os,
                         nuschl::testing::print_test const &pe);

std::ostream &operator<<(std::ostream &os,
                         const std::vector<nuschl::testing::print_test> &vec);

namespace boost::test_tools::tt_detail {

template <> struct print_log_value<nuschl::testing::print_test> {
    void operator()(std::ostream &os, nuschl::testing::print_test const &vec) {
        ::operator<<(os, vec);
    }
};

template <> struct print_log_value<std::vector<nuschl::testing::print_test>> {
    void operator()(std::ostream &os,
                    std::vector<nuschl::testing::print_test> const &vec) {
        ::operator<<(os, vec);
    }
};
}
