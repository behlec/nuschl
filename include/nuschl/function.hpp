#pragma once

#include <memory>
#include <string>
#include <vector>

#include <nuschl/environment_fwd.hpp>
#include <nuschl/s_exp_fwd.hpp>
#include <nuschl/symbol.hpp>

namespace nuschl {

//! A function
class lambda {
  public:
    using argument_list = std::vector<symbol>;

    lambda(argument_list argument_names, const s_exp_ptr function_body,
           env_ptr env);

    s_exp_ptr body() const noexcept;
    const argument_list &get_argument_names() const noexcept;
    env_ptr get_env() const noexcept;
    std::string representation() const;

    ~lambda() = default;

  private:
    const argument_list m_vars;
    const s_exp_ptr m_body;
    env_ptr m_env;
};

//! Pointer to lambda.
using lambda_ptr = std::shared_ptr<lambda>;

//! Print the lambda.
std::ostream &operator<<(std::ostream &, const lambda &);
}
