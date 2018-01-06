#include <nuschl/unittests/parse_res.hpp>

std::ostream &operator<<(std::ostream &os,
                         nuschl::testing::parse_example const &pe) {
    os << "{ \"" << pe.input << "\", " << *pe.expected << "}";
    return os;
}

std::ostream &
operator<<(std::ostream &os,
           const std::vector<nuschl::testing::parse_example> &vec) {
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
