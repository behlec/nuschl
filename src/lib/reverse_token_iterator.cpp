#include <nuschl/parsing/reverse_token_iterator.hpp>
#include <nuschl/parsing/token.hpp>

nuschl::parsing::reverse_token_iterator::reverse_token_iterator(
    const reverse_input_iterator &p, const reverse_input_iterator &end,
    const input_iterator &b)
    : m_pos(p), m_end(end), m_b(b),
      m_t(token::tokens::atom, m_b + (m_end - m_pos), m_b + (m_end - m_pos)) {
    parse();
}

nuschl::parsing::token nuschl::parsing::reverse_token_iterator::
operator*() const {
    return m_t;
}

const nuschl::parsing::token *nuschl::parsing::reverse_token_iterator::
operator->() const {
    return &m_t;
}

nuschl::parsing::reverse_input_iterator
nuschl::parsing::reverse_token_iterator::find_atom_end() const {
    reverse_input_iterator cur = m_pos;
    if (cur == m_end)
        return cur;
    ++cur;
    while ((cur != m_end) && !std::isspace(*cur) && !(*cur == '(') &&
           !(*cur == ')')) {
        ++cur;
    }
    return cur;
}

void nuschl::parsing::reverse_token_iterator::parse() {
    if (m_pos == m_end) {
        return;
    }
    if (*m_pos == '(') {
        m_fin = m_pos + 1;
        m_t = token(token::tokens::lparan, m_b + (m_end - m_fin),
                    m_b + (m_end - m_pos));
    } else if (*m_pos == ')') {
        m_fin = m_pos + 1;
        m_t = token(token::tokens::rparan, m_b + (m_end - m_fin),
                    m_b + (m_end - m_pos));
    } else {
        auto cur = find_atom_end();
        m_fin = cur;
        m_t = token(token::tokens::atom, m_b + (m_end - m_fin),
                    m_b + (m_end - m_pos));
    }
}

nuschl::parsing::reverse_token_iterator &
    nuschl::parsing::reverse_token_iterator::operator++() {
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

nuschl::parsing::reverse_token_iterator
    nuschl::parsing::reverse_token_iterator::operator++(int) {
    auto old = *this;
    ++*this;
    return old;
}

bool nuschl::parsing::operator==(const reverse_token_iterator &a,
                                 const reverse_token_iterator &b) {
    return a.m_pos == b.m_pos;
}

bool nuschl::parsing::operator!=(const reverse_token_iterator &a,
                                 const reverse_token_iterator &b) {
    return !(a == b);
}
