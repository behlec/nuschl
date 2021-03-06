#pragma once

/**
 * \file
 *
 * \brief Select boost::variant or std::variant depending on
 * NUSCHL_BOOSTVARIANT
 */

#ifdef NUSCHL_BOOSTVARIANT
#include <boost/variant.hpp>
namespace nuschl {
template <typename... Ts> using variant = boost::variant<Ts...>;
namespace vget = boost;
}
#else
#include <variant>
namespace nuschl {
template <typename... Ts> using variant = std::variant<Ts...>;
namespace vget = std;
}
#endif
