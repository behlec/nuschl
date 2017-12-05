#pragma once

/**
 * \file
 *
 * Because why should this be easy, it is C++.
 */

#include <fstream>
#include <string>

namespace nuschl::parsing {

/**
 * \brief Read file contents into string.
 *
 * \throws If any file operation fails.
 */
std::string read_file(const char *filename);

/**
 * \brief Read file contents into string.
 *
 * \throws If any file operation fails.
 */
std::string read_file(std::string filename);
}
