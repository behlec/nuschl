#include <iostream>
#include <sstream>

#include <nuschl/exceptions.hpp>
#include <nuschl/interpreter3.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <algorithm>
#include <cassert>

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

nuschl::interpreter3::interpreter3(const s_exp *prog, env_ptr env,
                                   memory::s_exp_pool *pool)
    : m_pool(pool), m_prog(prog), m_start_env(env) {}

const nuschl::s_exp *nuschl::interpreter3::run() {
    reset();
    compile_proc();
    assert(m_env_stack.size() == 1);
    eval();
    return m_regs.acc;
}

void nuschl::interpreter3::compile_proc() {
    // assert(m_arg_stack.empty());
    auto old_stack_size = m_arg_stack.size();
    auto l = list_size(m_regs.pc);
    push_list_on_stack(m_regs.pc, m_arg_stack);
    for (decltype(l) i = 0; i < l; ++i) {
        m_uneval_stack.push(m_arg_stack.top());
        m_arg_stack.pop();
        m_ops.push_back(op::evalarg);
    }
    // assert(m_arg_stack.empty());
    assert(m_arg_stack.size() == old_stack_size);
}

void nuschl::interpreter3::reset() {
    clear_stack(m_env_stack);
    m_env_stack.push(m_start_env);
    clear_stack(m_arg_stack);
    clear_stack(m_uneval_stack);
    clear_stack(m_op_stack);
    m_regs = {m_prog, nuschl::s_exp::nil};
}

void nuschl::interpreter3::push_stacks() {
    stacks tmp;
    tmp.arg_stack.swap(m_arg_stack);
    tmp.uneval_stack.swap(m_uneval_stack);
    m_stack_stack.push(tmp);
    push_ops();
    assert(m_uneval_stack.empty());
    assert(m_arg_stack.empty());
}

void nuschl::interpreter3::pop_stacks() {
    assert(!m_stack_stack.empty());
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
    assert(m_ops.empty());
    m_ops = m_op_stack.top();
    m_op_stack.pop();
}

void nuschl::interpreter3::eval_atom() {
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

void nuschl::interpreter3::eval_special() {
    auto exp = m_regs.pc;
    const s_exp *h = exp->car();
    std::string value = h->get_atom()->get_symbol().get_value();
    if ((value == "quote")) { // || (value == "'")) {
        m_regs.acc = exp->cdr()->car();
        return;
    } else if (value == "if") {
        auto cond = (exp->cdr()->car());
        auto cons = (exp->cdr()->cdr()->car());
        auto alt = (exp->cdr()->cdr()->cdr()->car());
        m_regs.pc = cond;
        m_uneval_stack.push(alt);
        m_uneval_stack.push(cons);
        m_uneval_stack.push(cond);
        push_ops();
        m_ops.push_back(op::evalarg);
        m_ops.push_back(op::ifexp);
        return;
    } else if (value == "define") {
        const s_exp *var = exp->cdr()->car();
        m_regs.pc = exp->cdr()->cdr()->car();
        eval_sexp();
        if (!(var->is_atom() && var->get_atom()->is_symbol())) {
            throw eval_error("Expected symbol as first argument", exp);
        }
        m_arg_stack.push(var);
        m_uneval_stack.push(m_regs.pc);
        push_ops();
        m_ops.push_back(op::evalarg);
        m_ops.push_back(op::define);
        return;
    } else if (value == "let") {
        // push_regs();
        push_stacks();
        assert(m_uneval_stack.empty());
        assert(m_arg_stack.empty());
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
        return;
    } else if (value == "lambda") {
        auto varlist = exp->cdr()->car();
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
        return;
    } else if (value == "begin") {
        m_regs.pc = exp->cdr();
        push_ops();
        compile_proc();
        return;
    }
    std::cerr << "Error: <" << value
              << "> not implemented as special command\n";
    throw interpreter_error(
        "Internal interpreter error. Special command not implemented.");
}

void nuschl::interpreter3::eval_list() {
    auto exp = m_regs.pc;
    if (is_empty_cell(m_regs.pc)) {
        m_regs.acc = m_regs.pc;
        return;
    }
    const s_exp *h = exp->car();
    if (h->is_atom() && h->get_atom()->is_symbol()) { // Test if it is special
        std::string value = h->get_atom()->get_symbol().get_value();
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
    // m_ops.push_back(op::ret);
}

void nuschl::interpreter3::eval() {
start:
    while (!m_ops.empty()) {
        auto o = m_ops.front();
        m_ops.pop_front();
        switch (o) {
        case op::popargs:
            assert(!m_arg_stack.empty());
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
            assert(!m_uneval_stack.empty());
            m_regs.pc = m_uneval_stack.top();
            m_uneval_stack.pop();
            eval_sexp();
            break;
        case op::pusharg:
            m_arg_stack.push(m_regs.acc);
            break;
        case op::define: {
            auto var = m_arg_stack.top();
            m_arg_stack.pop();
            m_env_stack.top()->set(var->get_atom()->get_symbol(), m_regs.acc);
            break;
        }
        case op::makeenv: {
            // assert(m_uneval_stack.empty());
            auto unevalsize = m_uneval_stack.size();
            assert(m_arg_stack.size() > 0);
            while (m_arg_stack.size() > 1) {
                auto t = m_arg_stack.top();
                m_arg_stack.pop();
                m_uneval_stack.push(t);
            }
            std::vector<const s_exp *> var_names;
            list_to_cont(m_arg_stack.top(), std::back_inserter(var_names));
            m_arg_stack.pop();
            assert(m_arg_stack.empty());
            assert(var_names.size() == m_uneval_stack.size() - unevalsize);
            if (!std::all_of(var_names.begin(), var_names.end(),
                             [](const s_exp *e) { return is_symbol(e); })) {
                throw interpreter_error(
                    "Number of variable names does not match number of values");
            }
            std::reverse(var_names.begin(), var_names.end());
            environment::table t;
            for (auto e : var_names) {
                t.insert({e->get_atom()->get_symbol(), m_uneval_stack.top()});
                m_uneval_stack.pop();
            }
            // assert(m_uneval_stack.empty());
            assert(m_uneval_stack.size() == unevalsize);
            env_ptr env = std::make_shared<environment>(t, m_env_stack.top());
            m_env_stack.push(env);
            break;
        }
        case op::ret:
            assert(!m_op_stack.empty());
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
                std::vector<const s_exp *> args;
                while (!m_arg_stack.empty()) {
                    args.push_back(m_arg_stack.top());
                    m_arg_stack.pop();
                }
                // auto env = m_regs.pc->get_lambda()->get_env(args);
                auto l = m_regs.pc->get_lambda();
                auto vars = l->get_argument_names();
                if (vars.size() != args.size()) {
                    throw eval_error("Wrong number of arguments to lambda",
                                     m_regs.pc);
                }
                environment::table t;
                for (decltype(vars)::size_type i = 0; i < vars.size(); ++i) {
                    t.insert({vars[i], args[i]});
                }
                auto env =
                    std::make_shared<nuschl::environment>(t, m_env_stack.top());
                m_env_stack.push(env);
                assert(m_arg_stack.empty());
                assert(m_uneval_stack.empty());
                m_regs.acc = s_exp::nil;
                m_regs.pc = m_regs.pc->get_lambda()->body();
                assert(m_ops.empty());
                compile_proc();
                m_ops.push_back(op::ret);
            } else {
                throw interpreter_error("Expected function on call stack");
            }
            break;
        default:
            std::cerr << "Opcode invalid or not implemented: " << o << '\n';
            throw interpreter_error("Opcode not implemented or invalid");
        }
    }
    assert(m_ops.empty());
    if (!m_op_stack.empty()) {
        pop_ops();
        goto start;
    }
    assert(m_env_stack.size() == 1);
}

void nuschl::interpreter3::eval_sexp() {
    switch (m_regs.pc->get_kind()) {
    // An atom is just evaluated, either it is a value or a variable.
    case s_exp::kind::atom:
        eval_atom();
        return;
        break;
    case s_exp::kind::primitive: // TODO why is this never triggered?
        throw eval_error("Do not know what to do with primitive",
                         m_regs.pc); // TODO correct? Or rather:
        m_regs.acc = m_regs.pc;
        return;
    case s_exp::kind::lambda: // TODO why is this never triggered?
        throw eval_error("Do not know what to do with lambda",
                         m_regs.pc); // TODO correct? Or rather:
        m_regs.acc = m_regs.pc;
        return;
    case s_exp::kind::cell:
        eval_list();
        return;
        break;
    default:
        throw interpreter_error(
            "Internel interpreter error. 0x23e"); // Should not happen
    }
}

void nuschl::interpreter3::push_empty_env() {
    env_ptr ignore = std::make_shared<environment>(m_env_stack.top());
    m_env_stack.push(ignore);
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
