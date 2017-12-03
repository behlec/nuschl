#include "helper.hpp"

std::unique_ptr<const nuschl::s_exp> nuschl::test::make_num(int i) {
    return std::make_unique<const nuschl::s_exp>(make_atom(number{i}));
}
