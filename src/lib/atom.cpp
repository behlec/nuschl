#include <nuschl/atom.hpp>

nuschl::atom::atom(const number &n) : m_kind(kind::number), m_value(n) {}

nuschl::atom::atom(const symbol &s) : m_kind(kind::symbol), m_value(s) {}
nuschl::atom::atom(symbol &&s) : m_kind(kind::symbol), m_value(std::move(s)) {}

bool nuschl::atom::is_number() const noexcept { return kind::number == m_kind; }
bool nuschl::atom::is_symbol() const noexcept { return kind::symbol == m_kind; }

const nuschl::number &nuschl::atom::get_number() const {
    return vget::get<nuschl::number>(m_value);
}

const nuschl::symbol &nuschl::atom::get_symbol() const {
    return vget::get<nuschl::symbol>(m_value);
}

bool nuschl::operator==(const nuschl::atom &a, const nuschl::atom &b) noexcept {
    return a.m_value == b.m_value;
}

bool nuschl::operator!=(const nuschl::atom &a, const nuschl::atom &b) noexcept {
    return a.m_value != b.m_value;
}

std::ostream &nuschl::operator<<(std::ostream &os, const nuschl::atom &a) {
    switch (a.m_kind) {
    case atom::kind::number:
        os << a.get_number();
        break;
    case atom::kind::symbol:
        os << a.get_symbol();
        break;
    }
    return os;
}

std::ostream &nuschl::operator<<(std::ostream &os, const nuschl::atom_ptr &ap) {
    os << ap.get();
    return os;
}
