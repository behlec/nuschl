#pragma once

#include <deque>
#include <stack>
#include <string>
#include <vector>

#include <nuschl/environment.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/s_exp.hpp>

namespace nuschl {

/**
 * \brief An interpreter that does not use recursion.
 *
 * Works by compiling the program into an internal set of commands. These
 * commands are then evaluated.
 *
 * The machine has the following data structures:
 * * Two registers, a stack for the registers.
 * * The current environment of variable bindings and a stack of it.
 * * Two stacks of S-expressions, the stack of arguments to evaluate and the
 *   stack of already evaluated arguments. A stack of both stacks.
 * * A list of operations to evaluate and a stack of those.
 * * A call stack of S-expressions.
 */
class interpreter3 {
  public:
    /**
     * \brief Set up an interpreter with a program.
     *
     * \param prog The program to execute.
     * \param initial_env The initial environment to use.
     * \param pool The memory pool to use.
     */
    interpreter3(const s_exp *prog, env_ptr initial_env,
                 memory::s_exp_pool *pool);

    //! \brief Run the program.
    //! \return The result of the run.
    const s_exp *run();

    /**
     * \brief Signal errors that are either internal or cannot directly
     * attributed to an expression.
     */
    class interpreter_error : public std::runtime_error {
      public:
        interpreter_error(const char *);
    };

  private:
    void reset();

    //! Evaluate opcodes.
    void eval();
    void eval_sexp();
    void eval_atom();
    void eval_list();
    void eval_special();

    void compile_func();
    void compile_proc();

    void push_empty_env();
    void push_ops();
    void pop_ops();

    //! Check if s is a special function that is handled by the interpreter.
    static bool is_special(const std::string &s);

    //! The list of special functions.
    static const std::vector<std::string> specials;

    memory::s_exp_pool *m_pool;

    //! The register set.
    struct regs {
        //! The program counter.
        const s_exp *pc = s_exp::nil;
        //! The accumulator register.
        const s_exp *acc = s_exp::nil;
    };

    //! The program.
    const s_exp *m_prog;
    //! The initial environment.
    env_ptr m_start_env;
    //! The registers
    regs m_regs;

    //! The internal instruction set.
    enum class op {
        //! Evaluate the top argument on the unevaluated stack.
        evalarg,
        //! Push the contents of the accumulator on the unevaluated stack.
        pusharg,
        /**
         * Load top of argument stack into accumulator and clear argument stack.
         */
        popargs,
        //! Evaluate an if expression.
        ifexp,
        //! Call the function on the call stack.
        call,
        //! Return from function. Pop stacks.
        ret,
        //! Push the program counter on the call stack.
        pushc,
        /**
         * Use the elements of the argument stack to create a new
         * binding.
         * The top n-1 elements are the arguments to the last element on the
         * argument stack. I.e. it has to be a list of symbol names.
         */
        makeenv,
        //! Set variable to value.
        define
    };
    using op_list = std::deque<op>;
    //! List of operation to execute.
    op_list m_ops;

    //! The stack
    std::stack<env_ptr> m_env_stack;
    //! Evaluated arguments.
    std::stack<const s_exp *> m_arg_stack;
    //! Unevaluated arguments.
    std::stack<const s_exp *> m_uneval_stack;

    std::stack<const s_exp *> m_call_stack;

    //! Save state of the current stacks.
    struct stacks {
        std::stack<const s_exp *> arg_stack;
        std::stack<const s_exp *> uneval_stack;
    };
    //! A stack of the argument stacks.
    std::stack<stacks> m_stack_stack;
    void push_stacks();
    void pop_stacks();

    std::stack<op_list> m_op_stack;

    friend std::ostream &operator<<(std::ostream &, interpreter3::op);
    friend std::ostream &operator<<(std::ostream &,
                                    const interpreter3::op_list &);
};

//! Print an opcode.
std::ostream &operator<<(std::ostream &, interpreter3::op);
//! Print a list of opcodes.
std::ostream &operator<<(std::ostream &, const interpreter3::op_list &);
}
