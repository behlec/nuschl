#pragma once

#include <nuschl/parsing/token.hpp>

#include <exception>

namespace nuschl::parsing {
class parse_error : public std::runtime_error {
  public:
    parse_error(const std::string &what, input_iterator pos);
    parse_error(std::string &&what, input_iterator pos);

    input_iterator where() const;

  private:
    input_iterator m_pos;
};
}
