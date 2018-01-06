#include <nuschl/symbol.hpp>

nuschl::symbol::symbol(const char *p) : m_value(p) {}

nuschl::symbol::symbol(const std::string &s) : m_value(s) {}

nuschl::symbol::symbol(std::string &&s) : m_value(std::move(s)) {}

const std::string &nuschl::symbol::get_value() const noexcept {
    return m_value;
}

bool nuschl::operator==(const symbol &a, const symbol &b) noexcept {
    return a.m_value == b.m_value;
}

bool nuschl::operator!=(const symbol &a, const symbol &b) noexcept {
    return a.m_value != b.m_value;
}

bool nuschl::symbol_cmp::operator()(const symbol &a, const symbol &b) const
    noexcept {
    return a.m_value < b.m_value;
}

std::ostream &nuschl::operator<<(std::ostream &os, const symbol &s) {
    os << s.m_value;
    return os;
}
