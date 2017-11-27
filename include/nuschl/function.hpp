#pragma once

#include <memory>
#include <string>
#include <vector>

#include <nuschl/s_exp_fwd.hpp>
#include <nuschl/symbol.hpp>

namespace nuschl {

class environment;

using env_ptr = std::shared_ptr<environment>;

//! Builtin primitive functions
class primitive {
  public:
    virtual s_exp_ptr
    execute(const std::vector<s_exp_ptr> &arguments) const = 0;
    virtual std::string representation() const = 0;
    virtual ~primitive() = default;
};

using primitive_ptr = std::shared_ptr<primitive>;

std::ostream &operator<<(std::ostream &, const primitive &);

//! A function
class lambda {
  public:
    using argument_list = std::vector<symbol>;

    lambda(argument_list argument_names, const s_exp_ptr function_body,
           env_ptr env);

    const s_exp_ptr body() const noexcept;
    const argument_list &get_argument_names() const noexcept;
    env_ptr get_env() const noexcept;
    std::string representation() const;

    ~lambda() = default;

  private:
    const argument_list m_vars;
    const s_exp_ptr m_body;
    env_ptr m_env;
};

using lambda_ptr = std::shared_ptr<lambda>;

std::ostream &operator<<(std::ostream &, const lambda &);
}
