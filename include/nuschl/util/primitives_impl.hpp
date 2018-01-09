#pragma once

#include <nuschl/exceptions.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/primitives.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <functional>

//! Helpers to implement the primitives.
namespace nuschl::primitive_impl {

//! Protoype for function to check the validity of arguments.
using checker_function =
    std::function<void(const std::vector<nuschl::s_exp_ptr> &)>;

//! Check the validity of arguments.
struct argument_checker {
    argument_checker(checker_function t) : c(t) {}

    void operator()(const std::vector<nuschl::s_exp_ptr> &args) const {
        c(args);
    }

    checker_function c;
};

//! Combine two argument_checkers. Both must accept.
argument_checker operator&&(const argument_checker &a,
                            const argument_checker &b);

//! Check if all arguments are numbers.
argument_checker all_numbers();
//! Check if there are exactly n arguments.
argument_checker exact_n_args(size_t n);
//! Check if there are at least n arguments.
argument_checker least_n_args(size_t n);
//! Expects one argument, which is a cell.
argument_checker is_list();

/**
 * \brief Turn sequence of numbers into a sequence of numbers.
 *
 * \tparam It Forward iterator
 * \tparam Ot Output iterator.
 * \param b Begin of input range.
 * \param e End of input range.
 * \param out Write ints to this iterator.
 */
template <typename It, typename Ot> void to_numbers(It b, It e, Ot out) {
    for (; b != e; ++b) {
        out = to_number(*b);
    }
}

/**
 * \brief Allows to build a primitive from a string, a function and a checker.
 */
template <typename F, typename T> class primitivebuilder : public primitive {
  public:
    using executor = F;
    using tester = T;

    /**
     * \brief Create a primitive.
     *
     * \param rep The representation.
     * \param f The function that implements the primitive.
     * \param t The checker for the arguments.
     */
    primitivebuilder(std::string rep, F f, T t)
        : m_representation(std::move(rep)), m_impl(f), m_test(t) {}

    /**
     * \brief Check the arguments with the checker and apply the function.
     *
     * Checks the arguments with the checker. Throws if arguments are not valid.
     * Then applies the function to the arguments.
     *
     * \param arguments The arguments to apply.
     * \param pool The memory pool to use to create new s_exp.
     * \throws eval_argument_error If the arguments are not OK.
     */
    s_exp_ptr execute(const std::vector<s_exp_ptr> &arguments,
                      memory::s_exp_pool *pool) const override {
        try {
            m_test(arguments);
        } catch (const nuschl::eval_argument_error &e) {
            std::string err_str = m_representation + " " + e.what();
            throw nuschl::eval_argument_error(err_str.c_str());
        }
        return m_impl(arguments, pool);
    }

    //! Return representation.
    virtual std::string representation() const override {
        return m_representation;
    }

  private:
    std::string m_representation;
    F m_impl;
    T m_test;
};

template <typename F, typename T>
primitivebuilder(T t, F f)->primitivebuilder<T, F>;
}
