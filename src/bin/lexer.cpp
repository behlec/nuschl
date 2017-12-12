#include <nuschl/parsing/lexer.hpp>
#include <nuschl/readfile.hpp>

#include <iostream>
#include <string>

using namespace nuschl;

std::string reverse_flag("--reverse");
std::string reverse_switch("-r");

void lex(const std::string &input) {
    nuschl::parsing::lexer l(input);
    for (auto t : l) {
        std::cout << t << ' ';
    }
    std::cout << '\n';
}

void rlex(const std::string &input) {
    nuschl::parsing::lexer l(input);
    for (auto it = l.rbegin(); it != l.rend(); ++it) {
        std::cout << *it << ' ';
    }
    std::cout << '\n';
}

int main(int argc, char **argv) {
    bool reverse = false;
    std::string input;
    try {
        if (argc == 1) {
            std::getline(std::cin, input);
        } else if (argc == 2) {
            reverse = true;
            if ((reverse_flag == argv[1]) || (reverse_switch == argv[1])) {
                std::getline(std::cin, input);
            } else {
                input = parsing::read_file(argv[1]);
            }
        } else if (argc == 3) {
            if ((reverse_flag == argv[1]) || (reverse_switch == argv[1])) {
                input = parsing::read_file(argv[2]);
                reverse = true;
            } else {
                std::cerr << "Unknown command line flag: " << argv[1] << '\n';
                exit(1);
            }
        } else {
            std::cerr << "Expecting zero or one arguments\n";
            return 1;
        }
    } catch (const std::runtime_error &e) {
        std::cerr << e.what() << '\n';
        return 1;
    }
    std::cout << input << '\n';
    std::cout << "Reverse:\n";
    if (reverse) {
        rlex(input);
    } else {
        lex(input);
    }
    return 0;
}
