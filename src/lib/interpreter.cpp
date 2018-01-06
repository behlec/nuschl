#include <nuschl/exceptions.hpp>
#include <nuschl/function.hpp>
#include <nuschl/interpreter.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <algorithm>
#include <cassert>
#include <iostream>
#include <sstream>

using namespace std::literals::string_literals;

nuschl::interpreter::interpreter(env_ptr env, memory::s_exp_pool *pool)
    : m_pool(pool) {
    m_env_stack.push(env);
}

const nuschl::s_exp *nuschl::interpreter::proc(const s_exp *exp) {
    assert(exp->is_cell());
    const s_exp *ret = s_exp::nil; // By default the return is nil.
    // Eval each item on the list, last result is the result.
    for_list(exp, [&ret, this](const s_exp *e) { ret = eval(e); });
    return ret;
}

const nuschl::s_exp *nuschl::interpreter::eval_atom(const s_exp *exp) {
    assert(exp->is_atom());
    auto a = exp->get_atom();
    if (a->is_symbol()) {
        try {
            return (*m_env_stack.top())[a->get_symbol()];
        } catch (const unbound_variable_error &e) {
            throw eval_error(("Unbound variable: "s + e.name()).c_str(), exp);
        }
    } else {
        return exp;
    }
}

bool nuschl::interpreter::is_special(const std::string &str) {
    return specials.end() != std::find(specials.begin(), specials.end(), str);
}

const nuschl::s_exp *nuschl::interpreter::eval_special(const s_exp *exp) {
    assert(exp->is_cell());
    const s_exp *h = exp->car();
    assert(is_symbol(h));
    std::string value = h->get_atom()->get_symbol().get_value();
    if ((value == "quote")) { // || (value == "'")) {
        return exp->cdr()->car();
    } else if (value == "if") {
        auto cond = (exp->cdr()->car());
        auto cons = (exp->cdr()->cdr()->car());
        auto alt = (exp->cdr()->cdr()->cdr()->car());
        if (to_bool(eval(cond))) {
            return eval(cons);
        } else {
            return eval(alt);
        }
    } else if (value == "define") {
        const s_exp *var = exp->cdr()->car();
        const s_exp *e = eval(exp->cdr()->cdr()->car());
        if (!(var->is_atom() && var->get_atom()->is_symbol())) {
            throw eval_error("Expected symbol as first argument", exp);
        }
        m_env_stack.top()->set(var->get_atom()->get_symbol(), e);
        return e;
    } else if (value == "let") {
        environment::table t;
        auto vars = exp->cdr()->car();
        for_list(vars, [&t, exp, this](const s_exp *var) {
            auto p = var;
            if (list_size(p) != 2) {
                std::stringstream s;
                s << "Expected pair, got ";
                s << *p;
                throw eval_error(s.str().c_str(), exp);
            }
            auto v = p->car();
            auto e = eval(p->cdr()->car());
            if (!(v->is_atom() && v->get_atom()->is_symbol())) {
                std::stringstream s;
                s << "Expected symbol, got ";
                s << *v;
                throw eval_error(s.str().c_str(), exp);
            }
            t.insert({v->get_atom()->get_symbol(), e});
        });
        push_new_env(t);
        auto ret = proc(exp->cdr()->cdr());
        m_env_stack.pop();
        return ret;
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
        return m_pool->create(f);
    } else if (value == "begin") {
        return proc(exp->cdr());
    }
    throw std::logic_error("Internal interpreter error. 0xBAE12");
}

const nuschl::s_exp *nuschl::interpreter::eval_list(const s_exp *exp) {
    assert(exp->is_cell());
    if (is_empty_cell(exp)) {
        return exp;
    }
    const s_exp *h = exp->car();
    if (is_symbol(h)) { // Test if it is special
        std::string value = h->get_atom()->get_symbol().get_value();
        if (is_special(value)) {
            return eval_special(exp);
        }
    }
    return eval_func(exp);
}

const nuschl::s_exp *nuschl::interpreter::eval_func(const s_exp *exp) {
    assert(exp->is_cell());
    // first element is function
    const s_exp *f = eval(exp->car());
    if (!(is_lambda(f) || is_primitive(f))) {
        throw eval_error("Expected function at first position", exp);
    }
    const s_exp *arg_list = exp->cdr();
    std::vector<const s_exp *> args; // The unevaluated arguments.
    list_to_cont(arg_list, std::back_inserter(args));
    std::vector<const s_exp *> eval_args; // The evaluated arguments.
    std::transform(args.begin(), args.end(), std::back_inserter(eval_args),
                   [this](const s_exp *e) { return eval(e); });
    try {
        if (f->is_lambda()) {
            auto l = f->get_lambda();
            // Create environment containing the arguments and pus it on the
            // environment stack.
            env_for_args(eval_args, l);
            auto e = l->body();
            auto res = proc(e);
            m_env_stack.pop(); // Pop arguments
            return res;

        } else {
            return f->get_primitive()->execute(eval_args, m_pool);
        }
    } catch (const eval_argument_error &e) {
        throw eval_error(e.what(), exp);
    }
}

const nuschl::s_exp *nuschl::interpreter::eval(const s_exp *exp) {
    // Just switch on the case.
    switch (exp->get_kind()) {
    // An atom is just evaluated, either it is a value or a variable.
    case s_exp::kind::atom:
        return eval_atom(exp);
        break;
    case s_exp::kind::primitive: // why is this never triggered?
        throw eval_error("Do not know what to do with primitive",
                         exp); // correct? Or rather:
        return exp;
    case s_exp::kind::lambda: // why is this never triggered?
        throw eval_error("Do not know what to do with lambda",
                         exp); // correct? Or rather:
        return exp;
    case s_exp::kind::cell:
        return eval_list(exp);
        break;
    default:
        throw std::logic_error(
            "Internal interpreter error. 0x23e"); // Should not happen
    }
}

const std::vector<std::string> nuschl::interpreter::specials = {
    "quote"s, "if"s, "define"s, "let"s, "lambda"s, "begin"s};

void nuschl::interpreter::env_for_args(const std::vector<const s_exp *> &args,
                                       lambda_ptr l) {
    lambda::argument_list arg_names(l->get_argument_names());
    if (args.size() > args.size()) {
        throw std::logic_error("Too many arguments for lambda");
    } else if (args.size() < args.size()) {
        throw std::logic_error("Too few arguments for lambda");
    }
    environment::table t;
    for (std::vector<const s_exp *>::size_type i = 0; i < arg_names.size();
         ++i) {
        t.insert({arg_names[i], args[i]});
    }
    push_new_env(t, l->get_env());
}

void nuschl::interpreter::push_new_env(const environment::table &t,
                                       env_ptr parent) {
    m_env_stack.push(std::make_shared<environment>(t, parent));
}

void nuschl::interpreter::push_new_env(const environment::table &t) {
    auto parent = m_env_stack.top();
    m_env_stack.push(std::make_shared<environment>(t, parent));
}