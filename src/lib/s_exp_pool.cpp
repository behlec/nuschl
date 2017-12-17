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
