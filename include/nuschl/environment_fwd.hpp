#pragma once

#include <memory>

/**
 * \file
 * \brief Forward declarations for environment.
 */

namespace nuschl {
class environment;

//! Pointer to point to environment.
using env_ptr = std::shared_ptr<environment>;
}
