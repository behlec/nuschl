#pragma once

#include <nuschl/s_exp.hpp>

#include <memory>

namespace nuschl::test {
std::unique_ptr<const s_exp> make_unique_num(int);
}
