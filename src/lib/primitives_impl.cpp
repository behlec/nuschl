#include <nuschl/util/primitives_impl.hpp>

#include <algorithm>

namespace nuschl::primitive_impl {
argument_checker operator&&(const argument_checker &a,
                            const argument_checker &b) {
    return argument_checker(
        checker_function([a, b](const std::vector<nuschl::s_exp_ptr> &args) {
            a(args);
            b(args);
        }));
}

argument_checker all_numbers() {
    return argument_checker([](const std::vector<s_exp_ptr> &args) -> void {
        if (std::all_of(args.begin(), args.end(), [](s_exp_ptr e) {
                return e->is_atom() && e->get_atom()->is_number();
            })) {
            return;
        }
        throw eval_argument_error("expects only numbers as arguments.");
    });
};

argument_checker exact_n_args(size_t n) {
    return argument_checker([n](const std::vector<s_exp_ptr> &args) {
        if (args.size() == n)
            return;

        std::string error_msg = "expects exactly " + std::to_string(n) +
                                "arguments, got " + std::to_string(args.size());
        throw eval_argument_error(error_msg.c_str());
    });
}

argument_checker least_n_args(size_t n) {
    return argument_checker([n](const std::vector<s_exp_ptr> &args) {
        if (args.size() >= n)
            return;

        std::string error_msg = "expects at least " + std::to_string(n) +
                                "arguments, got " + std::to_string(args.size());
        throw eval_argument_error(error_msg.c_str());
    });
}
}
