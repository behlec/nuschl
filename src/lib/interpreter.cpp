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

const nuschl::s_exp *nuschl::interpreter::eval_special_if(const s_exp *exp) {
    assert(is_cell(exp));
    auto args = exp->cdr();
    // Check arguments.
    if (!(is_cell(args) && list_size(args) > 1 && list_size(args) <= 3)) {
        throw eval_error("if requires two or three arguments", exp);
    }
    auto cond = (args->car());        // condition
    auto cons = (args->cdr()->car()); // consequent
    auto alt = s_exp::nil;            // Default alternate is nil
    if (list_size(args) == 3) {       // Check if altenate exists.
        alt = (args->cdr()->cdr()->car());
    }
    if (to_bool(eval(cond))) { // Evaluate condition
        return eval(cons);
    } else {
        return eval(alt);
    }
}

const nuschl::s_exp *nuschl::interpreter::eval_special_let(const s_exp *exp) {
    environment::table t; // For the newly bound variables.
    assert(is_cell(exp));
    // Check arguments.
    auto args = exp->cdr();
    if (!is_cell(args) || list_size(args) < 2) {
        throw eval_error(
            "Let requires one argument with the list of pairs and the body",
            exp);
    }
    auto defines = args->car();
    auto body = args->cdr();
    if (!is_cell(defines)) {
        throw eval_error("Let requires list as first arguments", exp);
    }
    // For each pair, create new binding.
    for_list(defines, [&t, exp, this](const s_exp *pair) {
        if (!is_cell(pair) || list_size(pair) != 2) {
            throw eval_error("Let requires list of pairs as argument", exp);
        }
        auto var = pair->car();
        assert(is_cell(pair->cdr()));
        auto e = eval(pair->cdr()->car());
        if (!(var->is_atom() && var->get_atom()->is_symbol())) {
            std::stringstream s;
            s << "Expected symbol as first part of pair, got ";
            s << *var;
            throw eval_error(s.str().c_str(), exp);
        }
        t.insert({var->get_atom()->get_symbol(), e});
    });
    // set up new environment.
    push_new_env(t);
    // Eval body
    auto ret = proc(body);
    m_env_stack.pop();
    return ret;
}

const nuschl::s_exp *
nuschl::interpreter::eval_special_lambda(const s_exp *exp) {
    auto args = exp->cdr();
    if (!is_cell(args) || list_size(args) < 2) {
        throw eval_error("Expect at least two arguments to lambda", exp);
    }
    auto varlist = args->car();
    if (!is_cell(varlist)) {
        throw eval_error("Expected list as first "
                         "argument to lambda",
                         exp);
    }
    std::vector<symbol> vec;
    // Get variable names.
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
}

const nuschl::s_exp *
nuschl::interpreter::eval_special_define(const s_exp *exp) {
    auto args = exp->cdr();
    if (!(is_cell(args) && list_size(args) > 1)) {
        throw eval_error("Define requires two arguments, got too few.", exp);
    }
    if (list_size(args) > 2) {
        throw eval_error("Define requires two arguments, got too many.", exp);
    }
    const s_exp *var = args->car();
    const s_exp *e = eval(args->cdr()->car());
    if (!(var->is_atom() && var->get_atom()->is_symbol())) {
        throw eval_error("Expected symbol as first argument", exp);
    }
    m_env_stack.top()->set(var->get_atom()->get_symbol(), e);
    return e;
}

const nuschl::s_exp *nuschl::interpreter::eval_special(const s_exp *exp) {
    assert(exp->is_cell());
    const s_exp *h = exp->car();
    assert(is_symbol(h));
    std::string value = h->get_atom()->get_symbol().get_value();
    if ((value == "quote")) { // || (value == "'")) {
        return exp->cdr()->car();
    } else if (value == "if") {
        return eval_special_if(exp);
    } else if (value == "define") {
        return eval_special_define(exp);
    } else if (value == "let") {
        return eval_special_let(exp);
    } else if (value == "lambda") {
        return eval_special_lambda(exp);
    } else if (value == "begin") {
        return proc(exp->cdr());
    }
    throw std::logic_error("Internal interpreter error. 0xBAE12");
}

const nuschl::s_exp *nuschl::interpreter::eval_list(const s_exp *exp) {
    if (is_empty_cell(exp)) {
        return exp;
    }
    assert(exp->is_cell());
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
            try {
                // Create environment containing the arguments and pus it on the
                // environment stack.
                env_for_args(eval_args, l);
            } catch (const std::logic_error &e) {
                throw eval_error(e.what(), exp);
            }
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
    if (args.size() > arg_names.size()) {
        throw std::logic_error("Too many arguments for lambda");
    } else if (args.size() < arg_names.size()) {
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
