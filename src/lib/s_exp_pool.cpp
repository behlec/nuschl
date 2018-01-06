#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/s_exp.hpp>

nuschl::memory::s_exp_pool::~s_exp_pool() { delete_handles(); }

void nuschl::memory::s_exp_pool::delete_handles() {
    for (auto e : m_handles)
        delete e;
}

void nuschl::memory::s_exp_pool::add(const s_exp *e) {
    m_handles.push_front(e);
}

const nuschl::s_exp *nuschl::memory::s_exp_pool::create_num(number n) {
    return create(make_atom(n));
}

const nuschl::s_exp *nuschl::memory::s_exp_pool::create_sym(symbol s) {
    return create(make_atom(std::move(s)));
}

const nuschl::s_exp *nuschl::memory::s_exp_pool::create_atom(number n) {
    return create(make_atom(n));
}

const nuschl::s_exp *nuschl::memory::s_exp_pool::create_atom(symbol s) {
    return create(make_atom(std::move(s)));
}
