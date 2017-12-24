#pragma once

#include <nuschl/s_exp_fwd.hpp>

#include <exception>
#include <string>

namespace nuschl {

//! \brief An error during eval.
class eval_error : public std::runtime_error {
  public:
    /**
     * \brief Report an error during evaluation of an expression.
     *
     * \param what A string describing the error.
     * \param e The expression that caused the error.
     */
    eval_error(const char *what, const s_exp_ptr e);
    s_exp_ptr exp() const noexcept;

  private:
    s_exp_ptr m_exp;
};

//! \brief An error evaluating the arguments for a primitive.
class eval_argument_error : public std::runtime_error {
  public:
    /**
     * \brief Report an error during evaluation of arguments.
     *
     * \param what A string describing the error.
     */
    explicit eval_argument_error(const char *what);
};

//! \brief Variable not defined.
class unbound_variable_error : public std::exception {
  public:
    explicit unbound_variable_error(const std::string &name);
    explicit unbound_variable_error(const std::string &&name);

    const std::string &name() const;

  private:
    const std::string m_name;
};
}
