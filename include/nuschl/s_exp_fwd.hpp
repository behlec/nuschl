#pragma once

#include <ostream>

namespace nuschl {
class s_exp;

using s_exp_ptr = const s_exp *;

std::ostream &operator<<(std::ostream &, const s_exp *);
}
