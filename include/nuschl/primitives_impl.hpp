#pragma once

#include <nuschl/exceptions.hpp>
#include <nuschl/primitives.hpp>
#include <nuschl/s_exp.hpp>

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

extern argument_checker all_numbers;
extern argument_checker exact_n_args(size_t n);
extern argument_checker least_n_args(size_t n);

/**
 * \brief Allows to build a primitive from a string, a function and a checker.
 */
template <typename F, typename T> class primitivebuilder : public primitive {
  public:
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
     * \throws eval_argument_error If the arguments are not OK.
     */
    s_exp_ptr execute(const std::vector<s_exp_ptr> &arguments) const override {
        try {
            m_test(arguments);
        } catch (const nuschl::eval_argument_error &e) {
            std::string err_str = m_representation + " " + e.what();
            throw nuschl::eval_argument_error(err_str.c_str());
        }
        return m_impl(arguments);
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
