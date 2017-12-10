#include <nuschl/parsing/lexer.hpp>

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
