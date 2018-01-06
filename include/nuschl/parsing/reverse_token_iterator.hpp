#pragma once

#include <nuschl/parsing/token.hpp>

namespace nuschl::parsing {
/**
* \brief Allows to iterate in reverse over the tokens.
*
* The external interface is, except from the constructor, equal to
* token_iterator.
*
* Always points to a valid token or to is the end.
*
* In the input stream the position p is actually the last position of the token.
* Besides the reverse end it holds also the position of the beginning of the
* input to return tokens correctly.
*/
class reverse_token_iterator {
  public:
    /**
     * \brief Create a token.
     *
     * \param p The start of the input.
     * \param end The end of the input.
     * \param b The end of the input.
     */
    reverse_token_iterator(const reverse_input_iterator &p,
                           const reverse_input_iterator &end,
                           const input_iterator &b);

    token operator*() const;
    const token *operator->() const;

    reverse_token_iterator &operator++();
    reverse_token_iterator operator++(int);

    friend bool operator==(const reverse_token_iterator &,
                           const reverse_token_iterator &);
    friend bool operator!=(const reverse_token_iterator &,
                           const reverse_token_iterator &);

  private:
    // fields
    reverse_input_iterator m_pos;
    reverse_input_iterator m_fin;
    reverse_input_iterator m_end;
    input_iterator m_b;
    token m_t;

    // methods
    /**
     * \brief Parse the next token.
     */
    void parse();
    /**
     * \brief if m_pos points to the start of an atom, return the end of the
     * atom.
     */
    reverse_input_iterator find_atom_end() const;
};

bool operator==(const reverse_token_iterator &, const reverse_token_iterator &);
bool operator!=(const reverse_token_iterator &, const reverse_token_iterator &);
}
