#pragma once

#include <nuschl/environment.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/s_exp.hpp>

#include <stack>
#include <string>
#include <vector>

namespace nuschl {

/**
 * \brief A simple interpreter.
 */
class interpreter {
  public:
    /**
     * \param env The initial environment to use.
     * \param pool The memory pool to use.
     */
    interpreter(env_ptr env, memory::s_exp_pool *pool);
    //! Evaluate a single expression.
    const s_exp *eval(const s_exp *);
    //! Evaluate a list of expressions.
    const s_exp *proc(const s_exp *);

  private:
    //! Evaluate an atom.
    const s_exp *eval_atom(const s_exp *);
    //! Evaluate a list. The result of the empty list is the empty list.
    const s_exp *eval_list(const s_exp *);
    //! Eval special builtin functions.
    const s_exp *eval_special(const s_exp *);
    //! Call lambda or primitive.
    const s_exp *eval_func(const s_exp *);

    //! Handle if
    const s_exp *eval_special_if(const s_exp *);
    //! Handle let
    const s_exp *eval_special_let(const s_exp *);
    //! Handle lambda
    const s_exp *eval_special_lambda(const s_exp *);
    //! Handle define
    const s_exp *eval_special_define(const s_exp *);

    //! Take the arguments to a lambda with the lambda and pushes a new
    //! environment with the arguments set on the environment stack.
    void env_for_args(const std::vector<const s_exp *> &args, lambda_ptr f);

    /**
     * \brief Create new environment and push it on the stack.
     *
     * Uses parent as parent.
     *
     * \param t Variable definitions for new environment.
     * \param parent Use as parent.
     */
    void push_new_env(const environment::table &t, env_ptr parent);
    /**
     * \brief Create new environment and push it on the stack.
     *
     * Uses current environment as parent.
     *
     * \param t Variable definitions for new environment.
     */
    void push_new_env(const environment::table &t);

    //! Test if a symbol is a special function.
    static bool is_special(const std::string &);

    //! Keep track of environments. Allow to add new bindings and forget them.
    std::stack<env_ptr> m_env_stack;
    memory::s_exp_pool *m_pool;

    //! List of special builtin functions.
    static const std::vector<std::string> specials;
};
}
