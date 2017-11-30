#pragma once

#include <nuschl/exceptions.hpp>
#include <nuschl/function.hpp>
#include <nuschl/s_exp.hpp>

#include <functional>

namespace nuschl::primitive_impl {

using checker_function =
    std::function<void(const std::vector<nuschl::s_exp_ptr> &)>;

struct argument_checker {
    argument_checker(checker_function t) : c(t) {}

    void operator()(const std::vector<nuschl::s_exp_ptr> &args) const {
        c(args);
    }

    checker_function c;
};

argument_checker operator&&(const argument_checker &a,
                            const argument_checker &b);

extern argument_checker all_numbers;
extern argument_checker exact_n_args(size_t n);
extern argument_checker least_n_args(size_t n);

template <typename F, typename T> class primitivebuilder : public primitive {
  public:
    primitivebuilder(std::string rep, F f, T t)
        : m_representation(std::move(rep)), m_impl(f), m_test(t) {}

    s_exp_ptr execute(const std::vector<s_exp_ptr> &arguments) const override {
        try {
            m_test(arguments);
        } catch (const nuschl::eval_argument_error &e) {
            std::string err_str = m_representation + " " + e.what();
            throw nuschl::eval_argument_error(err_str.c_str());
        }
        return m_impl(arguments);
    }

    virtual std::string representation() const override {
        return m_representation;
    }

    std::string m_representation;
    F m_impl;
    T m_test;
};

template <typename F, typename T>
primitivebuilder(T t, F f)->primitivebuilder<T, F>;
}
