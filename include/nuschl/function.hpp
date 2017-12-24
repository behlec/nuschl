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
    //! The arguments by name.
    using argument_list = std::vector<symbol>;

    /**
     * \brief Create a function.
     *
     * \param argument_names The names of the arguments.
     * \param function_body The body of the function.
     * \param env The variable bindings at the point of instantiation.
     */
    lambda(argument_list argument_names, const s_exp_ptr function_body,
           env_ptr env);

    //! Return the body of the function.
    s_exp_ptr body() const noexcept;
    //! Return the names of the arguments.
    const argument_list &get_argument_names() const noexcept;
    //! Return the environment at point of instantiation.
    env_ptr get_env() const noexcept;
    //! A string representation of the function.
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
