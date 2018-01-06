#pragma once

#include <nuschl/atom.hpp>
#include <nuschl/function.hpp>
#include <nuschl/primitives.hpp>
#include <nuschl/s_exp_fwd.hpp>
#include <nuschl/variant.hpp>

#include <iostream>

namespace nuschl {
/**
 * \brief An S-Expression.
 *
 * Can be a cell, i.e. pair of S-Expressions, or an atom, a primitive, or a
 * lambda.
 *
 * A list is represented by a cell, where  car = value, cdr = cell rest of list,
 * cdr = nil last element.
 *
 * Nil is a singleton instance. It is a cell with two pointers to itself.
 *
 * \attention While nil is logically equivalent to the empty list we have to
 * distinguish it internally.
 * Otherwise we cannot distinguish between the empty list and the list
 * containing nil.
 *
 */
class s_exp {
  public:
    //! The kind of S-Expressions.
    enum class kind { cell, atom, primitive, lambda };

    explicit s_exp(const atom_ptr &);
    explicit s_exp(const primitive_ptr &);
    explicit s_exp(const lambda_ptr &);
    s_exp(const s_exp *, const s_exp *);

    //! Get the kind of cell type.
    kind get_kind() const noexcept;

    //! Only valid if cell
    const s_exp *car() const;
    //! Only valid if cell
    const s_exp *cdr() const;

    const atom_ptr &get_atom() const;
    const primitive_ptr &get_primitive() const;
    const lambda_ptr &get_lambda() const;

    //! Is this instance nil.
    bool is_nil() const noexcept;
    bool is_atom() const noexcept;
    bool is_primitive() const noexcept;
    bool is_lambda() const noexcept;
    //! True iff of type cell and not nil.
    bool is_cell() const noexcept;

    friend std::ostream &operator<<(std::ostream &, const s_exp *);
    friend std::ostream &operator<<(std::ostream &, const s_exp *);

    friend bool operator==(const s_exp &, const s_exp &);
    friend bool operator!=(const s_exp &, const s_exp &);

    static const s_exp nil_elem;
    static const s_exp *nil;

    static const s_exp tru_elem;
    static const s_exp *tru;

    static const s_exp fals_elem;
    static const s_exp *fals;

  private:
    //! Nil.
    s_exp();

    using pair = std::pair<const s_exp *, const s_exp *>;
    kind m_kind;
    variant<atom_ptr, primitive_ptr, lambda_ptr, pair> m_value;
};

std::ostream &operator<<(std::ostream &, const s_exp &);

bool operator==(const s_exp &, const s_exp &);
bool operator!=(const s_exp &, const s_exp &);
}
