#include <nuschl/builtins.hpp>

nuschl::s_exp_ptr
nuschl::primitive_impl::list::execute(const std::vector<s_exp_ptr> &arguments,
                                      memory::s_exp_pool *pool) const {
    assert(pool != nullptr);
    auto res = pool->create(s_exp::nil, s_exp::nil);
    for (auto it = arguments.rbegin(); it != arguments.rend(); ++it) {
        res = pool->create(*it, res);
    }
    return res;
}

std::string nuschl::primitive_impl::list::representation() const {
    return "list";
}
