#include <nuschl/atom.hpp>
#include <nuschl/s_exp.hpp>

#include <stack>

nuschl::s_exp::s_exp() : m_kind(kind::cell), m_value(pair(nil, nil)) {}

nuschl::s_exp::s_exp(const atom_ptr &a) : m_kind(kind::atom), m_value(a) {}

nuschl::s_exp::s_exp(const primitive_ptr &p)
    : m_kind(kind::primitive), m_value(p) {}

nuschl::s_exp::s_exp(const lambda_ptr &l) : m_kind(kind::lambda), m_value(l) {}

nuschl::s_exp::s_exp(const s_exp *a, const s_exp *b)
    : m_kind(kind::cell), m_value(pair(a, b)) {}

nuschl::s_exp::kind nuschl::s_exp::get_kind() const noexcept { return m_kind; }

const nuschl::s_exp *nuschl::s_exp::car() const {
    return vget::get<pair>(m_value).first;
}

const nuschl::atom_ptr &nuschl::s_exp::get_atom() const {
    return vget::get<atom_ptr>(m_value);
}

const nuschl::primitive_ptr &nuschl::s_exp::get_primitive() const {
    return vget::get<primitive_ptr>(m_value);
}

const nuschl::lambda_ptr &nuschl::s_exp::get_lambda() const {
    return vget::get<lambda_ptr>(m_value);
}

const nuschl::s_exp *nuschl::s_exp::cdr() const {
    return vget::get<pair>(m_value).second;
}

bool nuschl::s_exp::is_nil() const noexcept {
    if (kind::cell != m_kind) {
        return false;
    }
    const s_exp *a = car();
    const s_exp *b = cdr();
    return (a == nil || a == nullptr) && (b == nil || b == nullptr);
}

bool nuschl::s_exp::is_atom() const noexcept { return kind::atom == m_kind; }

bool nuschl::s_exp::is_primitive() const noexcept {
    return kind::primitive == m_kind;
}

bool nuschl::s_exp::is_lambda() const noexcept {
    return kind::lambda == m_kind;
}

bool nuschl::s_exp::is_cell() const noexcept {
    if (kind::cell != m_kind) {
        return false;
    } else {
        return !is_nil();
    }
}

const nuschl::s_exp *nuschl::s_exp::nil = &nuschl::s_exp::nil_elem;
const nuschl::s_exp nuschl::s_exp::nil_elem(&nil_elem, &nil_elem);

std::ostream &nuschl::operator<<(std::ostream &os, const s_exp *e) {
    switch (e->get_kind()) {
    case s_exp::kind::atom:
        os << *e->get_atom();
        break;
    case s_exp::kind::primitive:
        os << *e->get_primitive();
        break;
    case s_exp::kind::lambda:
        os << *e->get_lambda();
        break;
    case s_exp::kind::cell:
        if (e->is_nil())
            os << "nil";
        else {
            os << "(";
            os << e->car();
            os << ", ";
            os << e->cdr();
            os << ")";
        }
        break;
    }
    return os;
}

std::ostream &nuschl::operator<<(std::ostream &os, const s_exp &e) {
    os << &e;
    return os;
}
