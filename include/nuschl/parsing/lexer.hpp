#pragma once

#include <nuschl/parsing/reverse_token_iterator.hpp>
#include <nuschl/parsing/token.hpp>
#include <nuschl/parsing/token_iterator.hpp>

#include <ostream>

namespace nuschl::parsing {

//! Position in input as line and column.
struct file_pos {
    std::string::size_type line;
    std::string::size_type column;
};

/**
 * \brief Split input into tokens. Does not really do anything, the real work is
 * done in token_iterator.
 *
 * \attention The input is not copy and must outlive the lexer instance and all
 * token_iterators that stem from it.
 */
class lexer {
  public:
    /**
     * \brief Create a lexer.
     *
     * \param input The input.
     * \attention The input is not copied. It must outlive the instance and all
     * token_iterators that stem from it.
     */
    lexer(const std::string &input);

    //! The first token.
    token_iterator begin() const;
    //! The end token, points to input.end().
    token_iterator end() const;

    reverse_token_iterator rbegin() const;
    //! The end token, points to input.end().
    reverse_token_iterator rend() const;

  private:
    input_iterator m_begin;
    input_iterator m_end;
    reverse_input_iterator m_rbegin;
    reverse_input_iterator m_rend;
};
}
