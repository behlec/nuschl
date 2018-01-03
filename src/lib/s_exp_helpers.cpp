#include <nuschl/util/s_exp_helpers.hpp>

bool nuschl::to_bool(const nuschl::s_exp *exp) {
    if (exp->is_nil())
        return false;
    if (exp->is_atom() && exp->get_atom()->is_symbol() &&
        (exp->get_atom()->get_symbol().get_value() == "#f"))
        return false;

    return true;
}

std::size_t nuschl::list_size(const nuschl::s_exp *e) {
    std::size_t ret = 0;
    while (!is_empty_cell(e)) {
        e = e->cdr();
        ++ret;
    }
    return ret;
}

bool nuschl::is_nil(const nuschl::s_exp *e) noexcept { return e->is_nil(); }

bool nuschl::is_symbol(const s_exp *e) noexcept {
    return e->is_atom() && e->get_atom()->is_symbol();
}

bool nuschl::is_number(const nuschl::s_exp *e) noexcept {
    return e->is_atom() && e->get_atom()->is_number();
}

bool nuschl::is_primitive(const s_exp *e) noexcept { return e->is_primitive(); }

bool nuschl::is_lambda(const s_exp *e) noexcept { return e->is_lambda(); }

bool nuschl::is_cell(const nuschl::s_exp *e) noexcept { return e->is_cell(); }
bool nuschl::is_empty_cell(const nuschl::s_exp *e) noexcept {
    if (is_nil(e)) {
        return true;
    } else if (!e->is_cell()) {
        return false;
    }
    return e->car()->is_nil() && e->cdr()->is_nil();
}

nuschl::number nuschl::to_number(const nuschl::s_exp *e) {
    return e->get_atom()->get_number();
}
