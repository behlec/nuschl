#pragma once

#include <nuschl/parsing/input.hpp>

namespace nuschl::parsing {

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
}
