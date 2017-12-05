#include <nuschl/readfile.hpp>

#include <exception>
#include <fstream>
#include <string>

std::string nuschl::parsing::read_file(const char *filename) try {
    std::ifstream in(filename, std::ios::in | std::ios::binary);
    in.exceptions(std::ifstream::failbit);
    if (!in.is_open()) {
        throw std::runtime_error("Could not open file:");
    }
    if (in) {
        std::string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return contents;
    } else {
        throw std::runtime_error("Could not read file contents:");
    }
} catch (std::runtime_error &e) {
    std::string err = e.what();
    err += filename;
    throw std::runtime_error(err);
}

std::string nuschl::parsing::read_file(std::string filename) {
    return read_file(filename.c_str());
}
