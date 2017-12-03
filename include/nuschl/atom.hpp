#pragma once

#include <iostream>
#include <memory>

#include <nuschl/number.hpp>
#include <nuschl/symbol.hpp>
#include <nuschl/variant.hpp>

namespace nuschl {
/**
 * \brief An atom is either a number or a symbol.
 */
class atom {
  public:
    //! Create atom from number.
    explicit atom(const number &);
    //! Create atom from symbol.
    explicit atom(const symbol &);
    //! Create atom from symbol.
    explicit atom(symbol &&);

    //! Is this atom a number?
    bool is_number() const noexcept;
    //! Is this atom a symbol?
    bool is_symbol() const noexcept;

    /**
     * \brief Retun the number.
     * \throws If atom is not a number.
     */
    const number &get_number() const;
    /**
     * \brief Retun the symbol.
     * \throws If atom is not a symbol.
     */
    const symbol &get_symbol() const;

    friend bool operator==(const atom &, const atom &) noexcept;
    friend bool operator!=(const atom &, const atom &) noexcept;
    friend std::ostream &operator<<(std::ostream &, const atom &);

  private:
    //! Kinds of atom
    enum class kind { number, symbol };
    //! The kind of the atom.
    kind m_kind;
    //! Holds the value.
    variant<symbol, number> m_value;
};

//! Pointer type to point to atoms.
using atom_ptr = std::shared_ptr<const atom>;

//! Create an atom from a number.
atom_ptr make_atom(const number &);
//! Create an atom from a symbol.
atom_ptr make_atom(const symbol &);
//! Create an atom from a symbol.
atom_ptr make_atom(const symbol &&);

/**
 * \brief Compare two atoms.
 *
 * Equal if of same type and the values are equal.
 */
bool operator==(const atom &, const atom &) noexcept;

/**
 * \brief Compare two atoms.
 *
 * Not equal if of different type or the values are not equal.
 */
bool operator!=(const atom &, const atom &) noexcept;

//! Print an atom
std::ostream &operator<<(std::ostream &, const atom &);

std::ostream &operator<<(std::ostream &, const atom_ptr &);
}
