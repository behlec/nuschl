#include <iostream>
#include <string>

#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/parsing/parser.hpp>

using namespace nuschl;

int main(int argc, char **argv) {
    memory::s_exp_pool pool;
    std::string input;
    if (argc > 2) {
        std::cout << "0 or 1 argument accepted!\n";
        exit(-1);
    } else if (argc == 2) {
        input = argv[1];
    } else {
        while (std::cin.good()) {
            std::string buffer;
            std::getline(std::cin, buffer);
            input += buffer;
        }
    }
    ::parsing::parser p(input, pool);

    auto res = p.parse();
    std::cout << "Parsed\n";
    if (res.ast == nullptr) {
        std::cout << "Empty input\n";
    } else {
        std::cout << *res.ast << std::endl;
    }
    return 0;
}
