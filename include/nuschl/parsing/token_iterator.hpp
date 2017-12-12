#pragma once

namespace nuschl::parsing {
/**
* \brief Allows to iterate over the tokens.
*
* Always points to a valid token or is the end.
*
* Holds a position to the start of the token to represent.
* Also holds a copy to end to do the iterations.
*/
class token_iterator {
  public:
    /**
     * \brief Create a token_iterator.
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
    // fields
    input_iterator m_pos;
    input_iterator m_fin;
    input_iterator m_end;
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
    input_iterator find_atom_end() const;
};
bool operator==(const token_iterator &, const token_iterator &);
bool operator!=(const token_iterator &, const token_iterator &);
}
