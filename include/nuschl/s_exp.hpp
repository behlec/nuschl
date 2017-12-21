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
 * Nil is a special s_exp of type cell with two pointers to itself.
 * Any cell where both s_exp are either nil or nullptr are considered nil.
 *
 * A list is represented by a cell, where  car = value, cdr = cell rest of list,
 * cdr = nil last element.
 */
class s_exp {
  public:
    //! The kind of S-Expressions.
    enum class kind { cell, atom, primitive, lambda };

    //! Nil.
    s_exp();
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
    bool is_cell() const noexcept;

    friend std::ostream &operator<<(std::ostream &, const s_exp *);
    friend std::ostream &operator<<(std::ostream &, const s_exp *);

    friend bool operator==(const s_exp &, const s_exp &);

    static const s_exp nil_elem;
    static const s_exp *nil;

  private:
    using pair = std::pair<const s_exp *, const s_exp *>;
    kind m_kind;
    variant<atom_ptr, primitive_ptr, lambda_ptr, pair> m_value;
};

std::ostream &operator<<(std::ostream &, const s_exp &);

bool operator==(const s_exp &, const s_exp &);
}
