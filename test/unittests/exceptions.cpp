#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/exceptions.hpp>
#include <nuschl/s_exp.hpp>

#include <sstream>
#include <string>

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(Exceptions)

BOOST_AUTO_TEST_CASE(EvalError) {
    nuschl::eval_error e("foo", nuschl::s_exp::nil);
    BOOST_CHECK_EQUAL("foo"s, e.what());
    BOOST_CHECK_EQUAL(*nuschl::s_exp::nil, *e.exp());
}

BOOST_AUTO_TEST_CASE(EvalArgumentError) {
    nuschl::eval_argument_error e("foo");
    BOOST_CHECK_EQUAL("foo"s, e.what());
}

BOOST_AUTO_TEST_CASE(UnboundVariableError) {
    nuschl::unbound_variable_error e("foo"s);
    BOOST_CHECK_EQUAL("Unbound variable: foo"s, e.what());
    BOOST_CHECK_EQUAL("foo"s, e.name());
    std::string s{"bar"};
    nuschl::unbound_variable_error e2(s);
    BOOST_CHECK_EQUAL("Unbound variable: bar"s, e2.what());
    BOOST_CHECK_EQUAL("bar"s, e2.name());
}

BOOST_AUTO_TEST_CASE(DynamicDestructor) {
    std::make_unique<nuschl::eval_error>("foo", nuschl::s_exp::nil);
    std::make_unique<nuschl::eval_argument_error>("foo");
    std::make_unique<nuschl::unbound_variable_error>("foo");
}

BOOST_AUTO_TEST_SUITE_END()
