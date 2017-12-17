#pragma once

#include <forward_list>
#include <memory>

#include <nuschl/s_exp.hpp>

namespace nuschl::memory {

class s_exp_pool {
  public:
    template <typename... T> const s_exp *create(T &&...);

    void add(const nuschl::s_exp *);

    ~s_exp_pool();

  private:
    std::forward_list<s_exp> m_elems;
    std::forward_list<const s_exp *> m_handles;

    void delete_handles();
};

template <typename... T> const s_exp *s_exp_pool::create(T &&... t) {
    m_elems.emplace_front(t...);
    return &(*m_elems.begin());
}
}
