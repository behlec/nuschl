#include <nuschl/unittests/s_exp_to_string.hpp>

std::ostream &operator<<(std::ostream &os,
                         nuschl::testing::s_exp_to_string const &pe) {
    os << "{ \"" << *pe.input << "\", " << pe.expected << "}";
    return os;
}

std::ostream &
operator<<(std::ostream &os,
           const std::vector<nuschl::testing::s_exp_to_string> &vec) {
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
