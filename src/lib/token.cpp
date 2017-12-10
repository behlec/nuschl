#include <nuschl/parsing/token.hpp>

nuschl::parsing::token::token(tokens type, input_iterator begin,
                              input_iterator end)
    : m_type(type), m_begin(begin), m_end(end) {}

bool nuschl::parsing::token::is_lparan() const noexcept {
    return tokens::lparan == m_type;
}

bool nuschl::parsing::token::is_rparan() const noexcept {
    return tokens::rparan == m_type;
}

bool nuschl::parsing::token::is_atom() const noexcept {
    return tokens::atom == m_type;
}

nuschl::parsing::token::tokens nuschl::parsing::token::type() const noexcept {
    return m_type;
}

std::string nuschl::parsing::token::value() const noexcept {
    return std::string(m_begin, m_end);
}

std::ostream &nuschl::parsing::operator<<(std::ostream &os, const token &t) {
    os << t.value();
    return os;
}
