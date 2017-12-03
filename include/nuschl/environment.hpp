#pragma once

#include <nuschl/environment_fwd.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/symbol.hpp>

#include <map>

namespace nuschl {
class environment {
  public:
    using table = std::map<symbol, const s_exp *, symbol_cmp>;

    environment();
    explicit environment(env_ptr);
    explicit environment(const table &, env_ptr);
    environment(const environment &) = delete;

    const s_exp *operator[](const symbol &) const;
    void set(const symbol &, const s_exp *);
    bool defined(const symbol &) const;

    env_ptr copy() const;

  private:
    env_ptr m_parent;
    table lookup;
};
}
