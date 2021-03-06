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
    return this == s_exp::nil;
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

const nuschl::s_exp *nuschl::s_exp::tru = &nuschl::s_exp::tru_elem;
const nuschl::s_exp nuschl::s_exp::tru_elem(make_atom("#t"));

const nuschl::s_exp *nuschl::s_exp::fals = &nuschl::s_exp::fals_elem;
const nuschl::s_exp nuschl::s_exp::fals_elem(make_atom("#f"));

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

bool nuschl::operator==(const s_exp &a, const s_exp &b) {
    if (a.get_kind() != b.get_kind()) {
        return false;
    }
    switch (a.get_kind()) {
    case s_exp::kind::atom:
        return *(a.get_atom()) == *(b.get_atom()); // compare values
        break;
    case s_exp::kind::primitive:
        return a.get_primitive() == b.get_primitive(); // compare addresses
        break;
    case s_exp::kind::lambda:
        return a.get_lambda() == b.get_lambda(); // compare addresses
        break;
    case s_exp::kind::cell:
        if (a.is_nil())
            return b.is_nil();
        else {
            return (*a.car() == *b.car()) && (*a.cdr() == *b.cdr());
        }
        break;
    }
    return false;
}

bool nuschl::operator!=(const s_exp &a, const s_exp &b) { return !(a == b); }
