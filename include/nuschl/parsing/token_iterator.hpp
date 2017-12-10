#pragma once

namespace nuschl::parsing {
/**
*\brief Allows to iterate over the tokens.
*
*Holds a position to the start of the token to represent.
*Also holds a copy to end to do the iterations.
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
}
