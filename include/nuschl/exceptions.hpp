#pragma once

#include <nuschl/s_exp_fwd.hpp>

#include <exception>

namespace nuschl {

class eval_error : public std::runtime_error {
  public:
    eval_error(const char *, const s_exp_ptr);
    const s_exp_ptr exp() const;

  private:
    const s_exp_ptr m_exp;
};

class eval_argument_error : public std::runtime_error {
  public:
    explicit eval_argument_error(const char *);
};
}
