#pragma once

#include <ostream>

namespace nuschl {
class number {
  public:
    explicit number(int);

    int get_value() const noexcept;

    friend number operator+(const number &, const number &) noexcept;
    friend number operator-(const number &, const number &) noexcept;
    friend number operator*(const number &, const number &)noexcept;
    friend number operator/(const number &, const number &) noexcept;

    friend number operator-(const number &) noexcept;

    friend bool operator==(const number &, const number &) noexcept;
    friend bool operator!=(const number &, const number &) noexcept;
    friend bool operator<(const number &, const number &) noexcept;
    friend bool operator>(const number &, const number &) noexcept;

    friend std::ostream &operator<<(std::ostream &, const number &);

  private:
    int m_value;
};

number operator+(const number &, const number &) noexcept;
number operator-(const number &, const number &) noexcept;
number operator*(const number &, const number &)noexcept;
number operator/(const number &, const number &) noexcept;

number operator-(const number &) noexcept;

bool operator==(const number &, const number &) noexcept;
bool operator!=(const number &, const number &) noexcept;
bool operator<(const number &, const number &) noexcept;
bool operator>(const number &, const number &) noexcept;

std::ostream &operator<<(std::ostream &, const number &);
}
