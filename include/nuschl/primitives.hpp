#pragma once

#include <nuschl/s_exp_fwd.hpp>

#include <memory>
#include <vector>

namespace nuschl {

namespace memory {
class s_exp_pool;
}

//! Builtin primitive functions
class primitive {
  public:
    /**
     * \brief Eval arguments.
     *
     * \param arguments The (already evaluated) arguments.
     * \throws eval_argument_error If the arguments are not as expected.
     */
    virtual s_exp_ptr execute(const std::vector<s_exp_ptr> &arguments,
                              memory::s_exp_pool *) const = 0;
    //! A representation for the primitive, used for printing or debugging.
    virtual std::string representation() const = 0;
    virtual ~primitive() = default;
};

//! Pointer to primitive.
using primitive_ptr = const primitive *;

//! Print the primitive, uses primitive::representation().
std::ostream &operator<<(std::ostream &, const primitive &);
}
