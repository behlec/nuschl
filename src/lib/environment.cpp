#include <nuschl/environment.hpp>
#include <nuschl/exceptions.hpp>

nuschl::environment::environment() : m_parent(nullptr) {
    lookup.insert(std::pair<nuschl::symbol, const s_exp *>("nil", s_exp::nil));
}

nuschl::environment::environment(env_ptr parent) : m_parent(parent) {}

nuschl::environment::environment(const table &t, env_ptr parent)
    : m_parent(parent), lookup(t) {}

nuschl::env_ptr nuschl::environment::copy() const {
    return std::make_shared<environment>(lookup, m_parent);
}

const nuschl::s_exp *nuschl::environment::operator[](const symbol &sym) const {
    if (auto it = lookup.find(sym); it != lookup.end()) {
        return it->second;
    } else if (m_parent) {
        return (*m_parent)[sym];
    } else {
        throw unbound_variable_error(sym.get_value());
        // return s_exp::nil;
    }
}

void nuschl::environment::set(const symbol &sym, const s_exp *exp) {
    lookup.insert_or_assign(sym, exp);
}

bool nuschl::environment::defined(const symbol &sym) const {
    return lookup.find(sym) != lookup.end();
}
