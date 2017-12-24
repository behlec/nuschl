#include <nuschl/unittests/vector_printer.hpp>

std::ostream &operator<<(std::ostream &os, const std::vector<int> &vec) {
    os << '[';
    bool first = true;
    for (int i : vec) {
        if (!first)
            os << ", ";
        os << i;
        first = false;
    }
    os << ']';
    return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::vector<std::string> &vec) {
    os << '[';
    bool first = true;
    for (const auto &i : vec) {
        if (!first)
            os << ", ";
        os << i;
        first = false;
    }
    os << ']';
    return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::vector<std::vector<std::string>> &vec) {
    os << '[';
    bool first = true;
    for (const auto &i : vec) {
        if (!first)
            os << ", ";
        os << i;
        first = false;
    }
    os << ']';
    return os;
}

std::ostream &operator<<(std::ostream &os,
                         const std::vector<const nuschl::s_exp *> &vec) {
    os << '[';
    bool first = true;
    for (auto i : vec) {
        if (!first)
            os << ", ";
        os << *i;
        first = false;
    }
    os << ']';
    return os;
}
