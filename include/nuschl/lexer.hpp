#pragma once

#include <ostream>
#include <string>

namespace nuschl::parsing {

using input_iterator = std::string::const_iterator;

//! Position in input as line and column.
struct file_pos {
    std::string::size_type line;
    std::string::size_type column;
};

/**
 * \brief A token of the input stream, including position data.
 *
 * There are three types, left paran, right paran, and atoms.
 * A token is the type plus source information from which the value (for atoms)
 * can be extracted.
 */
class token {
  public:
    enum class tokens { lparan, rparan, atom };
    token(tokens type, input_iterator begin, input_iterator end);

    bool is_lparan() const noexcept;
    bool is_rparan() const noexcept;
    bool is_atom() const noexcept;
    tokens type() const noexcept;

    //! Returns the characters of this token.
    std::string value() const noexcept;

  private:
    tokens m_type;
    input_iterator m_begin;
    input_iterator m_end;
};

//! Print the value.
std::ostream &operator<<(std::ostream &, const token &);

/**
 * \brief Allows to iterate over the tokens.
 *
 * Holds a position to the start of the token to represent.
 * Also holds a copy to end to do the iterations.
 */
class token_iterator {
  public:
    /**
     * \brief Create a token.
     *
     * \param p The start of the token.
     * \param end The end of the input.
     */
    token_iterator(const input_iterator &p, const input_iterator &end);

    token operator*() const;
    const token *operator->() const;

    token_iterator &operator++();
    token_iterator operator++(int);

    friend bool operator==(const token_iterator &, const token_iterator &);
    friend bool operator!=(const token_iterator &, const token_iterator &);

  private:
    void parse();
    input_iterator m_pos;
    input_iterator m_fin;
    input_iterator m_end;
    token m_t;
    input_iterator advance() const;
};

bool operator==(const token_iterator &, const token_iterator &);
bool operator!=(const token_iterator &, const token_iterator &);

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

  private:
    input_iterator m_begin;
    input_iterator m_end;
};
}
