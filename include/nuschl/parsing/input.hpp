#pragma once

#include <algorithm>
#include <iostream>
#include <string>

namespace nuschl::parsing {
using input_iterator = std::string::const_iterator;
using reverse_input_iterator = std::string::const_reverse_iterator;

/**
 * \brief Comput colum and row from iterators.
 *
 * Computes the number of lines between pos and b and the number of characters
 * of pos after the last '\n'.
 *
 * \param pos The position in the file stream.
 * \param b The beginning of the stream.
 * \param e The end of the stream.
 */
template <typename It>
std::pair<std::size_t, std::size_t> to_line(It pos, It b, It e) {
    It last_n = b;
    std::size_t no_lines = 0;
    It cur = b;
    while (cur != e) {
        last_n = cur;
        cur = std::find(cur, e, '\n');
        ++no_lines;
    }
    return std::make_pair(no_lines - 1, pos - last_n);
}

//! \brief Result of to_line as string.
template <typename It> std::string to_line_string(It pos, It b, It e) {
    std::string ret;
    auto p = to_line(pos, b, e);
    ret += std::to_string(p.first) + ":" + std::to_string(p.second);
    return ret;
}
}
