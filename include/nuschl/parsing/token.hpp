#pragma once

#include <string>

namespace nuschl::parsing {
using input_iterator = std::string::const_iterator;
using reverse_input_iterator = std::string::const_reverse_iterator;

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
