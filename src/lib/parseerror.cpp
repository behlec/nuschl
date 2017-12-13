#include <nuschl/parsing/parseerror.hpp>

nuschl::parsing::parse_error::parse_error(const std::string &what,
                                          input_iterator pos)
    : std::runtime_error(what) {}

nuschl::parsing::parse_error::parse_error(std::string &&what,
                                          input_iterator pos)
    : std::runtime_error(std::move(what)) {}

nuschl::parsing::input_iterator nuschl::parsing::parse_error::where() const {
    return m_pos;
}
