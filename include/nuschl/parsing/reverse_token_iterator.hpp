#pragma once

#include <nuschl/parsing/token.hpp>

namespace nuschl::parsing {
/**
*\brief Allows to iterate in reverse over the tokens.
*
*Holds a position to the start of the token to represent.
*Also holds a copy to end to do the iterations.
*/
class reverse_token_iterator {
  public:
    /**
     * \brief Create a token.
     *
     * \param p The start of the token.
     * \param end The end of the input.
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
    void parse();
    reverse_input_iterator find_atom_end() const;
};

bool operator==(const reverse_token_iterator &, const reverse_token_iterator &);
bool operator!=(const reverse_token_iterator &, const reverse_token_iterator &);
}
