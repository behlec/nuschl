#include <nuschl/lexer.hpp>
#include <nuschl/readfile.hpp>

#include <iostream>
#include <string>

using namespace nuschl;

void lex(const std::string &input) {
    nuschl::parsing::lexer l(input);
    for (auto it : l) {
        std::cout << it << ' ';
    }
    std::cout << '\n';
}

int main(int argc, char **argv) {
    std::string input;
    try {
        if (argc == 1) {
            std::getline(std::cin, input);
        } else if (argc == 2) {
            input = parsing::read_file(argv[1]);
        } else {
            std::cerr << "Expecting zero or one arguments\n";
            return 1;
        }
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    std::cout << input << '\n';
    lex(input);
    return 0;
}
