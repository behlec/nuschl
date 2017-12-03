#pragma once

#include <ostream>
#include <string>

namespace nuschl {

struct symbol_cmp;
/**
 * \brief An immutable string.
 */
class symbol {
  public:
    symbol(const char *);
    symbol(const std::string &);
    symbol(std::string &&);

    const std::string &get_value() const noexcept;

    friend bool operator==(const symbol &, const symbol &) noexcept;
    friend bool operator!=(const symbol &, const symbol &) noexcept;
    friend symbol_cmp;

    friend std::ostream &operator<<(std::ostream &, const symbol &);

  private:
    std::string m_value;
};

bool operator==(const symbol &, const symbol &) noexcept;
bool operator!=(const symbol &, const symbol &) noexcept;

//! Compare two symbols lexicographically.
struct symbol_cmp {
    bool operator()(const symbol &, const symbol &) const noexcept;
};

std::ostream &operator<<(std::ostream &, const symbol &);
}
