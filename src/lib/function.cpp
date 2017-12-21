#include <nuschl/function.hpp>

#include <sstream>

nuschl::lambda::lambda(lambda::argument_list argument_names,
                       const s_exp_ptr function_body, env_ptr env)
    : m_vars(argument_names), m_body(function_body), m_env(env) {}

nuschl::s_exp_ptr nuschl::lambda::body() const noexcept { return m_body; }

const nuschl::lambda::argument_list &nuschl::lambda::get_argument_names() const
    noexcept {
    return m_vars;
}

nuschl::env_ptr nuschl::lambda::get_env() const noexcept { return m_env; }

std::string nuschl::lambda::representation() const {
    std::stringstream s;
    s << "lambda closure (";
    s << m_vars.size();
    s << ")" << m_body;
    return s.str();
}

std::ostream &nuschl::operator<<(std::ostream &os, const lambda &l) {
    os << l.representation();
    return os;
}
