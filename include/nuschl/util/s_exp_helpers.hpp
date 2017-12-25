#pragma once

#include <stack>

#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/s_exp.hpp>

namespace nuschl {
/**
 * \brief Return size of list.
*/
std::size_t list_size(const s_exp *);

//! \brief Test if nil
bool is_nil(const s_exp *) noexcept;
//! \brief Test if nil
bool is_symbol(const s_exp *) noexcept;
//! \brief Test if number
bool is_number(const s_exp *) noexcept;
//! \brief Test if primitive
bool is_primitive(const s_exp *) noexcept;
//! \brief Test if lambda
bool is_lambda(const s_exp *) noexcept;
//! \brief Test if nil
bool is_cell(const s_exp *) noexcept;

//! \brief Test if lambda
number to_number(const s_exp *);

/**
 * \brief Convert s_exp to Boolean.
 *
 * \param e The expresion to convert.
 * \return false iff e is nil or "#f", true otherwise.
 */
bool to_bool(const s_exp *e);

/**
 * \brief Turn s-exp list into a container.
 *
 * \param e The list to transform.
 * \param it The output iterator to write to.
 * \tparam It Output iterator.
 */
template <typename It> void list_to_cont(const s_exp *e, It it);

/**
 * \brief Turn range to list.
 *
 * \param p Memory pool to use.
 * \param b Begin of range.
 * \param e End of range.
 * \tparam It Forward iterator.
 */
template <typename It> const s_exp *to_list(It b, It e, memory::s_exp_pool *p);
}

// - - - - - - - - - - - - - -
//     Implementation

template <typename It>
void nuschl::list_to_cont(const nuschl::s_exp *l, It out) {
    while (!(l->is_nil())) {
        *out = l->car();
        l = l->cdr();
    }
}

template <typename It>
const nuschl::s_exp *nuschl::to_list(It b, It e, memory::s_exp_pool *pool) {
    if (b == e) {
        return pool->create(s_exp::nil, s_exp::nil);
    }
    std::stack<const nuschl::s_exp *> s;
    const nuschl::s_exp *ret = nuschl::s_exp::nil;

    for (auto it = b; it != e; ++it) {
        s.push(*it);
    }
    while (!s.empty()) {
        auto n_ret = pool->create(s.top(), ret);
        ret = n_ret;
        s.pop();
    }
    return ret;
}