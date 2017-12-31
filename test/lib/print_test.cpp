#include <nuschl/unittests/print_test.hpp>

std::ostream &operator<<(std::ostream &os,
                         nuschl::testing::print_test const &pe) {
    os << "{ \"" << *pe.input << "\", " << pe.expected << "}";
    return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::vector<nuschl::testing::print_test> &vec) {
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
