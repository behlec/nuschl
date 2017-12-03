#pragma once

#include <ostream>

namespace nuschl {
class s_exp;

/**
 * Use where s_exp might not be directly reachable from interpreter.
 */
using s_exp_ptr = const s_exp *;

//! Print s_exp.
std::ostream &operator<<(std::ostream &, const s_exp *);
}
