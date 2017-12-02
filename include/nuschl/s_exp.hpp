#pragma once

#include <nuschl/atom.hpp>
#include <nuschl/function.hpp>
#include <nuschl/s_exp_fwd.hpp>
#include <nuschl/variant.hpp>

#include <iostream>

namespace nuschl {
class s_exp {
  public:
    enum class kind { cell, atom, primitive, lambda };

    s_exp();
    explicit s_exp(const atom_ptr &);
    explicit s_exp(const primitive_ptr &);
    explicit s_exp(const lambda_ptr &);
    s_exp(const s_exp *, const s_exp *);

    kind get_kind() const noexcept;

    const s_exp *car() const;
    const s_exp *cdr() const;

    const atom_ptr &get_atom() const;
    const primitive_ptr &get_primitive() const;
    const lambda_ptr &get_lambda() const;

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
