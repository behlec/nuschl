#pragma once

#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/parsing/token_iterator.hpp>
#include <nuschl/s_exp.hpp>

namespace nuschl::parsing {

// Result of a parse
struct parse_result {
    s_exp_ptr ast;
};

//! \brief Parse
class parser {
  public:
    parser(const std::string &input, memory::s_exp_pool &pool);

    parse_result parse();

  private:
    const std::string &m_input;
    memory::s_exp_pool &m_pool;
    /**
     * \brief Check if parantheses are corrct.
     *
     * Check if parantheses match and there are never more ')' than '('.
     */
    void check_parens(token_iterator, token_iterator) const;
};
}
