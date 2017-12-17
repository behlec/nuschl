#pragma once

#include <nuschl/s_exp_fwd.hpp>

#include <exception>
#include <stdexcept>
#include <string>

namespace nuschl {

//! An error during eval.
class eval_error : public std::runtime_error {
  public:
    eval_error(const char *what, const s_exp_ptr e);
    s_exp_ptr exp() const noexcept;

  private:
    s_exp_ptr m_exp;
};

//! An error evaluating the arguments for a primitive.
class eval_argument_error : public std::runtime_error {
  public:
    explicit eval_argument_error(const char *);
};

//! Variable not defined.
class unbound_variable_error : public std::exception {
  public:
    explicit unbound_variable_error(const std::string &name);
    explicit unbound_variable_error(const std::string &&name);

    const std::string &name() const;

  private:
    const std::string m_name;
};
}
