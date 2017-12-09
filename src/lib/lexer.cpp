#include <nuschl/lexer.hpp>

#include <cctype>

nuschl::parsing::token::token(tokens type, input_iterator from,
                              input_iterator to)
    : m_type(type), m_from(from), m_to(to) {}

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
    return std::string(m_from, m_to + 1);
}

std::ostream &nuschl::parsing::operator<<(std::ostream &os, const token &t) {
    os << t.value();
    return os;
}

nuschl::parsing::token_iterator::token_iterator(const input_iterator &p,
                                                const input_iterator &end)
    : m_pos(p), m_t(token::tokens::atom, m_pos, m_pos) {
    parse(end);
}

nuschl::parsing::token nuschl::parsing::token_iterator::operator*() const {
    return m_t;
}

const nuschl::parsing::token *nuschl::parsing::token_iterator::
operator->() const {
    return &m_t;
}

void nuschl::parsing::token_iterator::parse(const input_iterator &end) {
    if (*m_pos == '(') {
        m_fin = m_pos;
        m_t = token(token::tokens::lparan, m_fin, m_pos);
    } else if (*m_pos == ')') {
        m_fin = m_pos;
        m_t = token(token::tokens::rparan, m_fin, m_pos);
    } else {
        input_iterator cur = m_pos;
        while ((cur != end) && !std::isspace(*cur) && !(*cur == '(') &&
               (*cur == ')')) {
            cur++;
        }
        m_fin = cur;
        m_t = token(token::tokens::atom, m_pos, cur);
    }
}

bool nuschl::parsing::operator==(const token_iterator &a,
                                 const token_iterator &b) {
    return a.m_pos == b.m_pos;
}

bool nuschl::parsing::operator!=(const token_iterator &a,
                                 const token_iterator &b) {
    return !(a == b);
}

nuschl::parsing::lexer::lexer(const std::string &input)
    : m_begin(input.begin()), m_end(input.end()) {}

nuschl::parsing::token_iterator nuschl::parsing::lexer::begin() const {
    input_iterator res = m_begin;
    while (res != m_end && std::isspace(*res)) {
        ++res;
    }
    return token_iterator(res, m_end);
}

nuschl::parsing::token_iterator nuschl::parsing::lexer::end() const {
    return token_iterator(m_end, m_end);
}
