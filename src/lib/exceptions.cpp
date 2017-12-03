#include <nuschl/exceptions.hpp>

nuschl::eval_error::eval_error(const char *s, const s_exp_ptr e)
    : std::runtime_error(s), m_exp(e) {}

nuschl::s_exp_ptr nuschl::eval_error::exp() const noexcept { return m_exp; }

nuschl::eval_argument_error::eval_argument_error(const char *w)
    : std::runtime_error(w) {}

nuschl::unbound_variable_error::unbound_variable_error(const std::string &name)
    : m_name(name) {}

nuschl::unbound_variable_error::unbound_variable_error(const std::string &&name)
    : m_name(std::move(name)) {}

const std::string &nuschl::unbound_variable_error::name() const {
    return m_name;
}
