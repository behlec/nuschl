#pragma once

#include <iostream>

#include <nuschl/number.hpp>
#include <nuschl/symbol.hpp>
#include <nuschl/variant.hpp>

namespace nuschl {
class atom {
  public:
    explicit atom(const number &);
    explicit atom(const symbol &);
    explicit atom(symbol &&);

    bool is_number() const noexcept;
    bool is_symbol() const noexcept;

    const number &get_number() const;
    const symbol &get_symbol() const;

    friend bool operator==(const atom &, const atom &) noexcept;
    friend bool operator!=(const atom &, const atom &) noexcept;
    friend std::ostream &operator<<(std::ostream &, const atom &);

  private:
    enum class kind { number, symbol };
    kind m_kind;
    variant<symbol, number> m_value;
};

bool operator==(const atom &, const atom &) noexcept;
bool operator!=(const atom &, const atom &) noexcept;
std::ostream &operator<<(std::ostream &, const atom &);
}
