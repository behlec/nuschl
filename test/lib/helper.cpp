#include <nuschl/unittests/helper.hpp>

const nuschl::s_exp *nuschl::make_number(int i, memory::s_exp_pool *pool) {
    return pool->create(make_atom(number{i}));
}
