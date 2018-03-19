#pragma once

#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/parsing/parser.hpp>
#include <string>

namespace nuschl::testing {
class parse_string {
  public:
    parse_string(const std::string &input);
    const s_exp *operator()();

  private:
    std::string m_input;
    nuschl::memory::s_exp_pool m_pool;
};
}
