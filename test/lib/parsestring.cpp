#include <nuschl/unittests/parsestring.hpp>

nuschl::testing::parse_string::parse_string(const std::string &input)
    : m_input(input) {}

const nuschl::s_exp *nuschl::testing::parse_string::operator()() {
    nuschl::parsing::parser p(m_input, m_pool);
    return p.parse().ast;
}
