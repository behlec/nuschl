#pragma once

#include <nuschl/environment_fwd.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/symbol.hpp>

#include <map>

namespace nuschl {
/**
 * \brief Hold mapping of variables to values.
 *
 * A variable is defined by its name as a symbol.
 * The value is a s_exp.
 *
 * Since variables are lexically scoped environments can be "stacked".
 * An environment can have a pointer to another environment which is used as
 * fallback for he lookup.
 *
 * \see env_ptr
 */
class environment {
  public:
    //! Holds the mapping.
    using table = std::map<symbol, const s_exp *, symbol_cmp>;

    //! Create default environment, "nil" is defined.
    environment();
    /**
     * \brief Create environment with parent environment.
     *
     * Does not define any mapping. If parent is nullptr then no variable will
     * be defined.
     */
    explicit environment(env_ptr parent);
    /**
     * \brief Create environment with parent environment and given table.
     *
     * If t defines a variable that is also defined in parent than the mapping
     * of this instance is used instead of the one in parent.
     */
    explicit environment(const table &t, env_ptr parent);
    environment(const environment &) = delete;

    /**
     * \brief Look up variable.
     *
     * Returns the value of a variable if defined, else throws.
     * If var is not defined in this environment, search in parents if parent is
     * given.
     *
     * \param var The variable name.
     * \return The value of the variable.
     *
     * \throws unbound_variable_error If the variable is not defined.
     */
    const s_exp *operator[](const symbol &var) const;
    /**
     * \brief Assign an new value to a variable.
     *
     * If the variable is already defined the value is overwritten.
     * \param var The variable name
     * \param val The value of the variable.
     */
    void set(const symbol &var, const s_exp *val);
    //! Return if the variable is defined, includes ancestors.
    bool defined(const symbol &) const;
    //! Return if the variable is defined on this level of the environment.
    bool defined_locally(const symbol &) const;

    //! Create a copy.
    env_ptr copy() const;

  private:
    //! Parent pointer, nullable
    env_ptr m_parent;
    //! The local lookup table.
    table m_lookup;
};
}
