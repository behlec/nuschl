#include <nuschl/default_env.hpp>
#include <nuschl/exceptions.hpp>
#include <nuschl/interpreter3.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/parsing/parseerror.hpp>
#include <nuschl/parsing/parser.hpp>

#include <iostream>
#include <string>

using namespace nuschl;

int main(int argc, char **argv) {
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
    memory::s_exp_pool pool;
    parsing::parser p(input, pool);
    auto res = p.parse();
    // std::cout << "Parsed\n";
    if (res.ast == nullptr) {
        std::cout << "Empty input\n";
    } else {
        try {
            interpreter3 inter(res.ast, default_env.copy(), &pool);
            std::cout << *inter.run() << std::endl;
        } catch (const parsing::parse_error &e) {
            std::cout << e.what() << ' '
                      << parsing::to_line_string(e.where(), input.cbegin(),
                                                 input.cend())
                      << '\n';
        } catch (const eval_error &e) {
            std::cout << e.what() << '\n' << *e.exp() << '\n';
        }
    }
    return 0;
}
