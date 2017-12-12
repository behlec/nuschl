#include <nuschl/parsing/lexer.hpp>

nuschl::parsing::lexer::lexer(const std::string &input)
    : m_begin(input.begin()), m_end(input.end()), m_rbegin(input.rbegin()),
      m_rend(input.rend()) {}

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

nuschl::parsing::reverse_token_iterator nuschl::parsing::lexer::rbegin() const {
    reverse_input_iterator res = m_rbegin;
    while ((res != m_rend) && std::isspace(*res)) {
        ++res;
    }
    return reverse_token_iterator(res, m_rend, m_begin);
}

nuschl::parsing::reverse_token_iterator nuschl::parsing::lexer::rend() const {
    return reverse_token_iterator(m_rend, m_rend, m_begin);
}
