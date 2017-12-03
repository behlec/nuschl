#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/environment.hpp>
#include <nuschl/s_exp.hpp>

#include "helper.hpp"

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(Environment)

BOOST_AUTO_TEST_CASE(Default_constructor) {
    nuschl::environment e;

    BOOST_CHECK(*e["nil"s] == *nuschl::s_exp::nil);
    BOOST_TEST(!e.defined("a"s));
    BOOST_TEST(!e.defined("b"s));
}

BOOST_AUTO_TEST_CASE(Constructor_nullptr) {
    nuschl::environment e(nullptr);

    BOOST_TEST(!e.defined("a"s));
    BOOST_TEST(!e.defined("b"s));
}

BOOST_AUTO_TEST_CASE(simpl) {
    nuschl::environment::table t;
    auto a = nuschl::test::make_num(1);
    auto b = nuschl::test::make_num(2);

    t.insert({"a"s, a.get()});
    t.insert({"b"s, b.get()});
    nuschl::environment env1(t, nullptr);
    BOOST_TEST(*(env1["a"s]) == *nuschl::test::make_num(1));
    BOOST_TEST(*(env1["b"s]) == *nuschl::test::make_num(2));
}

BOOST_AUTO_TEST_CASE(lookup) {
    nuschl::environment env(nullptr);

    nuschl::environment::table t;
    auto a = nuschl::test::make_num(1);
    auto b = nuschl::test::make_num(2);

    t.insert({"a"s, a.get()});
    t.insert({"b"s, b.get()});
    nuschl::environment env1(t, nullptr);
    nuschl::environment::table t2;

    auto a2 = nuschl::test::make_num(23);
    t2.insert({"a"s, a2.get()});
    nuschl::env_ptr ep1(&env1, [](nuschl::environment *) {});
    nuschl::environment env2(t2, ep1);

    BOOST_TEST(*(env1["a"s]) == *nuschl::test::make_num(1));
    BOOST_TEST(*(env1["b"s]) == *nuschl::test::make_num(2));

    BOOST_TEST(*(env2["a"s]) == *nuschl::test::make_num(23));
    BOOST_TEST(*(env2["b"s]) == *nuschl::test::make_num(2));

    nuschl::environment env3(ep1);
    BOOST_TEST(*(env3["a"s]) == *nuschl::test::make_num(1));
    BOOST_TEST(*(env3["b"s]) == *nuschl::test::make_num(2));
}

BOOST_AUTO_TEST_SUITE_END()
