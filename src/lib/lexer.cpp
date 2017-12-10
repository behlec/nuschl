#include <nuschl/lexer.hpp>

#include <cctype>

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

nuschl::parsing::token_iterator::token_iterator(const input_iterator &p,
                                                const input_iterator &end)
    : m_pos(p), m_end(end), m_t(token::tokens::atom, m_pos, m_pos) {
    parse();
}

nuschl::parsing::token nuschl::parsing::token_iterator::operator*() const {
    return m_t;
}

const nuschl::parsing::token *nuschl::parsing::token_iterator::
operator->() const {
    return &m_t;
}

nuschl::parsing::input_iterator
nuschl::parsing::token_iterator::advance() const {
    input_iterator cur = m_pos;
    if (cur == m_end)
        return cur;
    ++cur;
    while ((cur != m_end) && !std::isspace(*cur) && !(*cur == '(') &&
           !(*cur == ')')) {
        ++cur;
    }
    return cur;
}

void nuschl::parsing::token_iterator::parse() {
    if (*m_pos == '(') {
        m_fin = m_pos + 1;
        m_t = token(token::tokens::lparan, m_pos, m_fin);
    } else if (*m_pos == ')') {
        m_fin = m_pos + 1;
        m_t = token(token::tokens::rparan, m_pos, m_fin);
    } else {
        auto cur = advance();
        m_fin = cur;
        m_t = token(token::tokens::atom, m_pos, m_fin);
    }
}

nuschl::parsing::token_iterator &nuschl::parsing::token_iterator::operator++() {
    if (m_fin == m_end) { // we where the last token
        m_pos = m_end;    // set to end
        return *this;
    }
    if (isspace(*m_fin)) { // we need to skip whitespace
        m_pos = m_fin;
        while ((m_pos != m_end) && std::isspace(*m_pos)) {
            ++m_pos;
        }
    } else { // we are not at the end and point at the begin of the next token
        m_pos = m_fin;
    }
    parse();
    return *this;
}

nuschl::parsing::token_iterator nuschl::parsing::token_iterator::
operator++(int) {
    auto old = *this;
    ++*this;
    return old;
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
    while ((res != m_end) && std::isspace(*res)) {
        ++res;
    }
    return token_iterator(res, m_end);
}

nuschl::parsing::token_iterator nuschl::parsing::lexer::end() const {
    return token_iterator(m_end, m_end);
}
