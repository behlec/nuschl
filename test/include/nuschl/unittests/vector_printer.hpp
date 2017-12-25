#pragma once

// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/test_tools.hpp>
// clang-format on

#include <nuschl/s_exp.hpp>

#include <iostream>
#include <vector>

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec);
std::ostream &operator<<(std::ostream &os, const std::vector<std::string> &vec);
std::ostream &operator<<(std::ostream &os,
                         const std::vector<std::vector<std::string>> &vec);
std::ostream &operator<<(std::ostream &os,
                         const std::vector<const nuschl::s_exp *> &vec);

namespace boost {
namespace test_tools {
namespace tt_detail {

template <> struct print_log_value<std::vector<int>> {
    void operator()(std::ostream &os, std::vector<int> const &vec) {
        ::operator<<(os, vec);
    }
};

template <> struct print_log_value<std::vector<std::string>> {
    void operator()(std::ostream &os, std::vector<std::string> const &vec) {
        ::operator<<(os, vec);
    }
};

template <> struct print_log_value<std::vector<std::vector<std::string>>> {
    void operator()(std::ostream &os,
                    std::vector<std::vector<std::string>> const &vec) {
        ::operator<<(os, vec);
    }
};

template <> struct print_log_value<std::vector<const nuschl::s_exp *>> {
    void operator()(std::ostream &os,
                    std::vector<const nuschl::s_exp *> const &vec) {
        ::operator<<(os, vec);
    }
};
}
}
}
