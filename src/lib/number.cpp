#include <nuschl/number.hpp>

nuschl::number::number(int i) : m_value(i) {}

int nuschl::number::get_value() const noexcept { return m_value; }

nuschl::number nuschl::operator+(const number &a, const number &b) noexcept {
    return number(a.m_value + b.m_value);
}
nuschl::number nuschl::operator-(const number &a, const number &b) noexcept {
    return number(a.m_value - b.m_value);
}
nuschl::number nuschl::operator*(const number &a, const number &b) noexcept {
    return number(a.m_value * b.m_value);
}
nuschl::number nuschl::operator/(const number &a, const number &b) noexcept {
    return number(a.m_value / b.m_value);
}

nuschl::number nuschl::operator-(const number &a) noexcept {
    return number(-a.m_value);
}

bool nuschl::operator==(const number &a, const number &b) noexcept {
    return a.m_value == b.m_value;
}
bool nuschl::operator!=(const number &a, const number &b) noexcept {
    return a.m_value != b.m_value;
}
bool nuschl::operator<(const number &a, const number &b) noexcept {
    return a.m_value < b.m_value;
}
bool nuschl::operator>(const number &a, const number &b) noexcept {
    return a.m_value > b.m_value;
}

std::ostream &nuschl::operator<<(std::ostream &os, const number &n) {
    os << n.m_value;
    return os;
}
