#include <iostream>
#include <sstream>

#include <nuschl/exceptions.hpp>
#include <nuschl/interpreter3.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <algorithm>

using namespace std::literals::string_literals;

namespace {
//! Empty stack by swap idiom
template <typename T> void clear_stack(std::stack<T> &s) {
    std::stack<T> tmp;
    s.swap(tmp);
}

/**
 * \brief Push a list of S-expressions on a stack.
 *
 * \param l The list of S-Expressions
 * \param st The stack to push on.
 */
void push_list_on_stack(const nuschl::s_exp *l,
                        std::stack<const nuschl::s_exp *> &st) {
    for_list(l, [&st](const nuschl::s_exp *e) { st.push(e); });
}
}

nuschl::interpreter3::interpreter_error::interpreter_error(const char *what)
    : runtime_error(what) {}

nuschl::interpreter3::interpreter_error::interpreter_error(
    const std::string &what)
    : runtime_error(what) {}

nuschl::interpreter3::interpreter3(const s_exp *prog, env_ptr env,
                                   memory::s_exp_pool *pool)
    : m_pool(pool), m_prog(prog), m_start_env(env) {}

const nuschl::s_exp *nuschl::interpreter3::run() {
    reset();
    compile_proc();
    internal_assert(m_env_stack.size() == 1,
                    "Huh, woody Hedghog? Env stack should be reset.");
    eval();
    return m_regs.acc;
}

void nuschl::interpreter3::compile_proc() {
    auto old_stack_size = m_arg_stack.size();
    auto l = list_size(m_regs.pc);
    push_list_on_stack(m_regs.pc, m_arg_stack);
    for (decltype(l) i = 0; i < l; ++i) {
        m_uneval_stack.push(m_arg_stack.top());
        m_arg_stack.pop();
        m_ops.push_back(op::evalarg);
    }
    internal_assert(
        m_arg_stack.size() == old_stack_size,
        "Huh, purple Ramblings? Argument stack size should not have changed.");
}

void nuschl::interpreter3::reset() {
    // Clear compiled options
    m_ops.clear();
    // Clear all stacks
    clear_stack(m_env_stack);
    clear_stack(m_arg_stack);
    clear_stack(m_uneval_stack);
    clear_stack(m_call_stack);
    clear_stack(m_stack_stack);
    clear_stack(m_op_stack);
    // Set up initial environment
    m_env_stack.push(m_start_env->copy());
    // Reset registers
    m_regs = {m_prog, nuschl::s_exp::nil};
}

void nuschl::interpreter3::push_stacks() {
    stacks new_top;
    new_top.arg_stack.swap(m_arg_stack);
    new_top.uneval_stack.swap(m_uneval_stack);
    m_stack_stack.push(new_top);
    push_ops();
    internal_assert(m_uneval_stack.empty(),
                    "Huh, rainbow Carrot? Uneval stack should be empty.");
    internal_assert(m_arg_stack.empty(),
                    "Huh, honky trident? Eval stack should be empty.");
}

void nuschl::interpreter3::pop_stacks() {
    internal_assert(!m_stack_stack.empty(),
                    "Huh, Happy Toad? Why pop empty stacks?");
    m_arg_stack.swap(m_stack_stack.top().arg_stack);
    m_uneval_stack.swap(m_stack_stack.top().uneval_stack);
    m_stack_stack.pop();
    pop_ops();
}

void nuschl::interpreter3::push_ops() {
    m_op_stack.push(m_ops);
    m_ops.clear();
}

void nuschl::interpreter3::pop_ops() {
    internal_assert(m_ops.empty(), "Huh, Irritating Cry? Poping even though "
                                   "the operation list is not empty?");
    internal_assert(!m_op_stack.empty(),
                    "Huh, Wry Dog?. Why pop empty operation stack?");
    m_ops = m_op_stack.top();
    m_op_stack.pop();
}

void nuschl::interpreter3::eval_atom() {
    internal_assert(m_regs.pc->is_atom(),
                    "Huh, Solid Stitch? Atom expected in eval_atom.");
    auto a = m_regs.pc->get_atom();
    if (a->is_symbol()) {
        try {
            m_regs.acc = (*m_env_stack.top())[a->get_symbol()];
            return;
        } catch (const unbound_variable_error &e) {
            throw eval_error(("Unbound variable: "s + e.name()).c_str(),
                             m_regs.pc);
        }
    } else {
        m_regs.acc = m_regs.pc;
        return;
    }
}

bool nuschl::interpreter3::is_special(const std::string &str) {
    return specials.end() != std::find(specials.begin(), specials.end(), str);
}

void nuschl::interpreter3::eval_special_if() {
    auto exp = m_regs.pc;
    auto args = exp->cdr();
    if (!(args->is_cell() && list_size(args) > 1 && list_size(args) < 4)) {
        throw eval_error("if requires two or three arguments", exp);
    }
    auto cond = (args->car());
    auto cons = (args->cdr()->car());
    auto alt = (args->cdr()->cdr()->car());
    m_regs.pc = cond;
    m_uneval_stack.push(alt);
    m_uneval_stack.push(cons);
    m_uneval_stack.push(cond);
    push_ops();
    m_ops.push_back(op::evalarg);
    m_ops.push_back(op::ifexp);
}

void nuschl::interpreter3::eval_special_define() {
    auto exp = m_regs.pc;
    auto args = exp->cdr();
    if (!is_cell(args)) {
        throw eval_error("Define expects list as arguments", exp);
    }
    if (list_size(args) < 2) {
        throw eval_error("Define requires two arguments, got too few.", exp);
    }
    if (list_size(args) > 2) {
        throw eval_error("Define requires two arguments, got too many.", exp);
    }
    const s_exp *var = args->car();
    m_regs.pc = args->cdr()->car();
    if (!(var->is_atom() && var->get_atom()->is_symbol())) {
        throw eval_error("Expected symbol as first argument", exp);
    }
    m_arg_stack.push(var);
    m_uneval_stack.push(m_regs.pc);
    push_ops();
    m_ops.push_back(op::evalarg);
    m_ops.push_back(op::define);
}

void nuschl::interpreter3::eval_special_let() {
    // push_regs();
    push_stacks();
    internal_assert(m_uneval_stack.empty(), "Huh, Ludicrous Lead?");
    internal_assert(m_arg_stack.empty(), "Huh, Psychedelic Plough?");
    auto exp = m_regs.pc;
    auto args = exp->cdr();
    if (list_size(args) < 2) {
        throw eval_error(
            "Let requires one argument with the list of pairs and the body",
            exp);
    }
    m_regs.pc = args->cdr();
    compile_proc();
    auto tail = m_ops;
    m_ops.clear();
    auto vars = args->car();
    if (!vars->is_cell()) {
        throw eval_error("Let requires list as first arguments", exp);
    }
    std::vector<const s_exp *> var_names;
    for_list(vars, [&](const nuschl::s_exp *pair) {
        if (list_size(pair) != 2) {
            throw eval_error("Let requires list of pairs as argument", exp);
        }

        auto v = pair->car();
        if (!(v->is_atom() && v->get_atom()->is_symbol())) {
            std::stringstream s;
            s << "Expected symbol as first part of pair, got ";
            s << *v;
            throw eval_error(s.str().c_str(), exp);
        }
        var_names.push_back(v);
        m_uneval_stack.push(pair->cdr()->car());
        m_ops.push_back(op::evalarg);
        m_ops.push_back(op::pusharg);
    });
    m_arg_stack.push(to_list(var_names.begin(), var_names.end(), m_pool));
    m_ops.push_back(op::makeenv);
    std::copy(tail.begin(), tail.end(), std::back_inserter(m_ops));
    m_ops.push_back(op::ret);
}

void nuschl::interpreter3::eval_special_lambda() {
    auto exp = m_regs.pc;
    auto args = exp->cdr();
    if (!(args->is_cell() && list_size(args) > 1)) {
        throw eval_error("Expect at least two arguments to lambda", exp);
    }
    auto varlist = exp->cdr()->car();
    if (!varlist->is_cell()) {
        throw eval_error("Expected list as first argument to lambda", exp);
    }
    std::vector<symbol> vec;
    for_list(varlist, [&vec, exp](const s_exp *var) {
        if (!(var->is_atom() && var->get_atom()->is_symbol())) {
            throw eval_error("Expected list of symbols as first "
                             "argument to lambda",
                             exp);
        }
        vec.push_back(var->get_atom()->get_symbol());
    });
    auto f =
        std::make_shared<lambda>(vec, exp->cdr()->cdr(), m_env_stack.top());
    m_regs.acc = m_pool->create(f);
}

void nuschl::interpreter3::eval_special() {
    auto exp = m_regs.pc;
    const s_exp *h = exp->car();
    std::string value = h->get_atom()->get_symbol().get_value();
    if ((value == "quote")) { // || (value == "'")) {
        m_regs.acc = exp->cdr()->car();
        return;
    } else if (value == "if") {
        eval_special_if();
        return;
    } else if (value == "define") {
        eval_special_define();
        return;
    } else if (value == "let") {
        eval_special_let();
        return;
    } else if (value == "lambda") {
        eval_special_lambda();
        return;
    } else if (value == "begin") {
        m_regs.pc = exp->cdr();
        push_ops();
        compile_proc();
        return;
    }
    throw interpreter_error("Special command not implemented.");
}

void nuschl::interpreter3::eval_list() {
    auto exp = m_regs.pc;
    if (is_empty_cell(m_regs.pc)) {
        m_regs.acc = m_regs.pc;
        return;
    }
    const s_exp *head = exp->car();
    if (is_symbol(head)) { // Test if it is special
        std::string value = head->get_atom()->get_symbol().get_value();
        if (is_special(value)) {
            eval_special();
            return;
        }
    }
    compile_func();
    return;
}

void nuschl::interpreter3::compile_func() {
    push_stacks();
    auto exp = m_regs.pc;
    const s_exp *arg_list = exp->cdr();
    m_ops.push_back(op::evalarg);
    m_ops.push_back(op::pushc);
    push_list_on_stack(arg_list, m_uneval_stack);
    for (auto i = m_uneval_stack.size(); i > 0; --i) {
        m_ops.push_back(op::evalarg);
        m_ops.push_back(op::pusharg);
    }
    m_regs.pc = exp->car();
    m_uneval_stack.push(m_regs.pc);
    m_ops.push_back(op::call);
}

void nuschl::interpreter3::eval() {
start:
    while (!m_ops.empty()) {
        auto o = m_ops.front();
        m_ops.pop_front();
        switch (o) {
        case op::popargs:
            internal_assert(!m_arg_stack.empty(),
                            "Huh, Deadpan Dad? Pop the empty operation stack?");
            m_regs.acc = m_arg_stack.top();
            clear_stack(m_arg_stack);
            break;
        case op::pushc:
            if (!(m_regs.acc->is_primitive() || m_regs.acc->is_lambda())) {
                throw interpreter_error("Must be function");
            }
            m_call_stack.push(m_regs.acc);
            break;
        case op::evalarg:
            internal_assert(!m_uneval_stack.empty(),
                            "Huh, Alike Alley? Pop the empty operation stack?");
            m_regs.pc = m_uneval_stack.top();
            m_uneval_stack.pop();
            eval_sexp();
            break;
        case op::pusharg:
            m_arg_stack.push(m_regs.acc);
            break;
        case op::define: {
            auto var = m_arg_stack.top();
            internal_assert(is_symbol(var),
                            "Huh, Earsplitting Muscle? Expected "
                            "variable name on argument stack.");
            m_arg_stack.pop();
            m_env_stack.top()->set(var->get_atom()->get_symbol(), m_regs.acc);
            break;
        }
        case op::makeenv: {
            auto unevalsize = m_uneval_stack.size();
            internal_assert(m_arg_stack.size() > 0, "Huh, Smooth Snakes?");
            while (m_arg_stack.size() > 1) {
                auto t = m_arg_stack.top();
                m_arg_stack.pop();
                m_uneval_stack.push(t);
            }
            std::vector<const s_exp *> var_names;
            list_to_cont(m_arg_stack.top(), std::back_inserter(var_names));
            m_arg_stack.pop();
            internal_assert(m_arg_stack.empty(), "Huh, Ripe Robin?");
            internal_assert(
                var_names.size() == m_uneval_stack.size() - unevalsize,
                "Huh, Accessible Attraction? Names and values should match.");
            if (!std::all_of(var_names.begin(), var_names.end(),
                             [](const s_exp *e) { return is_symbol(e); })) {
                throw interpreter_error("Variable names must be symbols");
            }
            std::reverse(var_names.begin(), var_names.end());
            environment::table t;
            for (auto e : var_names) {
                t.insert({e->get_atom()->get_symbol(), m_uneval_stack.top()});
                m_uneval_stack.pop();
            }
            internal_assert(m_uneval_stack.size() == unevalsize,
                            "Huh, Uptight Uncle?");
            env_ptr env = std::make_shared<environment>(t, m_env_stack.top());
            m_env_stack.push(env);
            break;
        }
        case op::ret:
            internal_assert(
                !m_op_stack.empty(),
                "Huh, Barbarous Brain? How to pop the empty operation stack?");
            pop_stacks();
            m_env_stack.pop();
            break;
        case op::ifexp:
            if (!to_bool(m_regs.acc)) {
                m_uneval_stack.pop();
                m_regs.pc = m_uneval_stack.top();
                m_uneval_stack.pop();
            } else {
                m_regs.pc = m_uneval_stack.top();
                m_uneval_stack.pop();
                m_uneval_stack.pop();
            }
            eval_sexp();
            break;
        case op::call:
            if (m_call_stack.empty()) {
                throw interpreter_error("Call stack empty on call");
            }
            m_regs.pc = m_call_stack.top();
            m_call_stack.pop();
            if (is_primitive(m_regs.pc)) {
                push_empty_env();
                std::vector<const s_exp *> args;
                while (!m_arg_stack.empty()) {
                    args.push_back(m_arg_stack.top());
                    m_arg_stack.pop();
                }
                try {
                    m_regs.acc =
                        m_regs.pc->get_primitive()->execute(args, m_pool);
                } catch (const eval_argument_error &e) {
                    throw eval_error(e.what(), m_regs.pc);
                }
                m_ops.push_back(op::ret);
            } else if (is_lambda(m_regs.pc)) {
                std::vector<const s_exp *> args; // The argument values
                while (!m_arg_stack.empty()) {
                    args.push_back(m_arg_stack.top());
                    m_arg_stack.pop();
                }
                // auto env = m_regs.pc->get_lambda()->get_env(args);
                auto l = m_regs.pc->get_lambda();
                auto vars = l->get_argument_names(); // The parameter names
                if (vars.size() < args.size()) {
                    throw eval_error("Too many arguments for lambda",
                                     m_regs.pc);
                } else if (vars.size() > args.size()) {
                    throw eval_error("Too few arguments for lambda", m_regs.pc);
                }
                environment::table t;
                for (decltype(vars)::size_type i = 0; i < vars.size(); ++i) {
                    t.insert({vars[i], args[i]});
                }
                auto env =
                    std::make_shared<nuschl::environment>(t, m_env_stack.top());
                m_env_stack.push(env);
                internal_assert(m_arg_stack.empty(), "Huh, Shallow Scale?");
                internal_assert(m_uneval_stack.empty(), "Huh, Fancy Fall?");
                m_regs.acc = s_exp::nil;
                m_regs.pc = m_regs.pc->get_lambda()->body();
                internal_assert(m_ops.empty(), "Huh, Sparkling Stem?");
                compile_proc();
                m_ops.push_back(op::ret);
            } else {
                throw interpreter_error("Expected function on call stack");
            }
            break;
        default:
            throw interpreter_error("Opcode not implemented or invalid");
        }
    }
    internal_assert(m_ops.empty(), "Huh, Spotless Soap?");
    if (!m_op_stack.empty()) {
        pop_ops();
        goto start;
    }
    internal_assert(
        m_env_stack.size() == 1,
        "Huh, Scandalous Suggestion? Expect environment to be back to normal?");
}

void nuschl::interpreter3::eval_sexp() {
    switch (m_regs.pc->get_kind()) {
    // An atom is just evaluated, either it is a value or a variable.
    case s_exp::kind::atom:
        eval_atom();
        return;
        break;
    case s_exp::kind::primitive: // TODO why is this never triggered?
        throw interpreter_error(
            "Do not know what "
            "to do with primitive in "
            "eval_sexp(). Supreme Lettuce"); // TODO correct? Or
                                             // rather:
        m_regs.acc = m_regs.pc;
        return;
    case s_exp::kind::lambda: // TODO why is this never triggered?
        throw interpreter_error(
            "Do not know what "
            "to do with lambda in eval_sexp(). Abortive Friends"); // TODO
        // correct?
        // Or rather:
        m_regs.acc = m_regs.pc;
        return;
    case s_exp::kind::cell:
        eval_list();
        return;
        break;
    default:
        throw interpreter_error("Crude Rhino."); // Should not happen
    }
}

void nuschl::interpreter3::push_empty_env() {
    env_ptr ignore = std::make_shared<environment>(m_env_stack.top());
    m_env_stack.push(ignore);
}

void nuschl::interpreter3::internal_assert(bool condition,
                                           const std::string &message) {
    if (!condition)
        throw interpreter_error("Internal interpreter error: "s + message);
}

void nuschl::interpreter3::internal_assert(bool condition,
                                           const char *message) {
    if (!condition)
        throw interpreter_error("Internal interpreter error: "s + message);
}

const std::vector<std::string> nuschl::interpreter3::specials = {
    "quote"s, "if"s, "define"s, "let"s, "lambda"s, "begin"s};

std::ostream &nuschl::operator<<(std::ostream &os, nuschl::interpreter3::op o) {
    switch (o) {
    case interpreter3::op::evalarg:
        os << "evalarg";
        break;
    case interpreter3::op::pusharg:
        os << "pusharg";
        break;
    case interpreter3::op::popargs:
        os << "popargs";
        break;
    case interpreter3::op::ifexp:
        os << "ifexp";
        break;
    case interpreter3::op::call:
        os << "call";
        break;
    case interpreter3::op::ret:
        os << "ret";
        break;
    case interpreter3::op::pushc:
        os << "pushc";
        break;
    case interpreter3::op::makeenv:
        os << "makenv";
        break;
    case interpreter3::op::define:
        os << "define";
        break;
    default:
        throw "op oops 0xBEC";
    }
    return os;
}

std::ostream &nuschl::operator<<(std::ostream &os,
                                 const nuschl::interpreter3::op_list &opl) {
    os << '[';
    for (auto o : opl) {
        os << o << ",";
    }
    os << ']';
    return os;
}
