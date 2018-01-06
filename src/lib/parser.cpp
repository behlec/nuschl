#include <nuschl/parsing/lexer.hpp>
#include <nuschl/parsing/parseerror.hpp>
#include <nuschl/parsing/parser.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <cassert>
#include <stack>

nuschl::parsing::parser::parser(const std::string &input,
                                memory::s_exp_pool &pool)
    : m_input(input), m_pool(pool) {}

void nuschl::parsing::parser::check_parens(token_iterator b,
                                           token_iterator e) const {
    int n = 0;
    for (token_iterator it = b; it != e; ++it) {
        if (it->is_lparan())
            ++n;
        else if (it->is_rparan())
            --n;
        if (n < 0) {
            throw parse_error("Too many closing parentheses", it->position());
        }
    }
    if (n != 0) {
        throw parse_error("Premature end of input expected )", e->position());
    }
}

nuschl::parsing::parse_result nuschl::parsing::parser::parse() {
    lexer l(m_input);
    check_parens(l.begin(), l.end());

    const s_exp *list_end = m_pool.create(s_exp::nil, s_exp::nil);
    assert(is_empty_cell(list_end));

    s_exp_ptr ret = s_exp::nil;

    std::stack<s_exp_ptr> stack;
    stack.push(list_end);

    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        if (it->is_rparan()) {
            stack.push(list_end);
        } else if (it->is_atom()) {
            atom_ptr a;
            assert(it->value().size() > 0);
            // FIXME this is fragile.
            if (isdigit(it->value()[0])) {
                a = make_atom(number(stoi(it->value())));
            } else {
                a = make_atom(it->value());
            }
            assert(!stack.empty());
            s_exp_ptr ca = m_pool.create(a);
            s_exp_ptr cd = stack.top();
            stack.pop();
            stack.push(m_pool.create(ca, cd));
            ret = stack.top();
        } else { // it->is_lparan()
            s_exp_ptr ca = stack.top();
            stack.pop();
            s_exp_ptr cd = stack.top();
            stack.pop();
            stack.push(m_pool.create(ca, cd));
            ret = stack.top();
        }
    }
    assert(!stack.empty());
    ret = stack.top();
    stack.pop();
    assert(stack.empty());
    return parse_result{ret};
}
