#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/function.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

#include <sstream>

using namespace nuschl;

namespace {
class pseudo : public primitive {
    s_exp_ptr execute(const std::vector<s_exp_ptr> &,
                      memory::s_exp_pool *) const override {
        const s_exp *ret = s_exp::nil;
        return ret;
    };
    std::string representation() const override { return "nil_prim"; };
};
}

BOOST_AUTO_TEST_SUITE(Primitive)

BOOST_AUTO_TEST_CASE(Ostream) {
    std::stringstream ss;
    ss << pseudo{};
    BOOST_CHECK_EQUAL(ss.str(), "nil_prim");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(Lambda)

symbol x{"x"};
auto a = std::make_shared<atom>(x);

s_exp e(a);
s_exp b(&e, s_exp::nil);

lambda::argument_list args = {x};
lambda l(args, &b, nullptr);

BOOST_AUTO_TEST_CASE(Constructor) {
    BOOST_CHECK(l.get_argument_names() == args);
    BOOST_CHECK(l.get_env() == nullptr);
}

BOOST_AUTO_TEST_CASE(Ostream) {
    std::stringstream ss;
    ss << l;
    BOOST_CHECK_EQUAL(ss.str(), "lambda closure (1)(x, nil)");
}

BOOST_AUTO_TEST_SUITE_END()
