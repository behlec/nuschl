#define BOOST_CHECK_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/environment.hpp>
#include <nuschl/s_exp.hpp>

#include <nuschl/unittests/unique.hpp>

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(Environment)

BOOST_AUTO_TEST_CASE(Default_constructor) {
    nuschl::environment e;

    BOOST_CHECK(*e["nil"s] == *nuschl::s_exp::nil);
    BOOST_CHECK(!e.defined("a"s));
    BOOST_CHECK(!e.defined("b"s));
    BOOST_CHECK(e.defined("nil"s));
    BOOST_CHECK(!e.defined_locally("a"s));
    BOOST_CHECK(!e.defined_locally("b"s));
    BOOST_CHECK(e.defined_locally("nil"s));
}

BOOST_AUTO_TEST_CASE(Constructor_nullptr) {
    nuschl::environment e(nullptr);

    BOOST_CHECK(!e.defined("a"s));
    BOOST_CHECK(!e.defined("b"s));
    BOOST_CHECK(!e.defined("nil"s));
    BOOST_CHECK(!e.defined_locally("a"s));
    BOOST_CHECK(!e.defined_locally("b"s));
    BOOST_CHECK(!e.defined_locally("nil"s));
}

BOOST_AUTO_TEST_CASE(Simpl) {
    nuschl::environment::table t;
    auto a = nuschl::test::make_unique_num(1);
    auto b = nuschl::test::make_unique_num(2);

    t.insert({"a"s, a.get()});
    t.insert({"b"s, b.get()});
    nuschl::environment env1(t, nullptr);
    BOOST_CHECK(env1.defined("a"s));
    BOOST_CHECK(env1.defined("b"s));

    BOOST_CHECK(env1.defined_locally("a"s));
    BOOST_CHECK(env1.defined_locally("b"s));

    BOOST_CHECK(*(env1["a"s]) == *nuschl::test::make_unique_num(1));
    BOOST_CHECK(*(env1["b"s]) == *nuschl::test::make_unique_num(2));
}

BOOST_AUTO_TEST_CASE(Lookup) {
    nuschl::environment env(nullptr);

    nuschl::environment::table t;
    auto a = nuschl::test::make_unique_num(1);
    auto b = nuschl::test::make_unique_num(2);

    t.insert({"a"s, a.get()});
    t.insert({"b"s, b.get()});
    nuschl::environment env1(t, nullptr);
    nuschl::environment::table t2;

    auto a2 = nuschl::test::make_unique_num(23);
    t2.insert({"a"s, a2.get()});
    nuschl::env_ptr ep1(&env1, [](nuschl::environment *) {});
    nuschl::environment env2(t2, ep1);

    BOOST_CHECK(*(env1["a"s]) == *nuschl::test::make_unique_num(1));
    BOOST_CHECK(*(env1["b"s]) == *nuschl::test::make_unique_num(2));

    BOOST_CHECK(*(env2["a"s]) == *nuschl::test::make_unique_num(23));
    BOOST_CHECK(*(env2["b"s]) == *nuschl::test::make_unique_num(2));

    nuschl::environment env3(ep1);
    BOOST_CHECK(*(env3["a"s]) == *nuschl::test::make_unique_num(1));
    BOOST_CHECK(*(env3["b"s]) == *nuschl::test::make_unique_num(2));
}
BOOST_AUTO_TEST_CASE(Defined) {
    nuschl::environment env(nullptr);

    nuschl::environment::table t;
    auto a = nuschl::test::make_unique_num(1);
    auto b = nuschl::test::make_unique_num(2);
    auto c = nuschl::test::make_unique_num(3);
    auto d = nuschl::test::make_unique_num(3);

    t.insert({"a"s, a.get()});
    t.insert({"b"s, b.get()});
    nuschl::environment env1(t, nullptr);
    nuschl::environment::table t2;
    t2.insert({"b"s, c.get()});
    t2.insert({"c"s, d.get()});
    nuschl::env_ptr ep1(&env1, [](nuschl::environment *) {});
    nuschl::environment env2(t2, ep1);

    BOOST_CHECK(env1.defined("a"s));
    BOOST_CHECK(env1.defined("b"s));
    BOOST_CHECK(!env1.defined("c"s));

    BOOST_CHECK(env1.defined_locally("a"s));
    BOOST_CHECK(env1.defined_locally("b"s));
    BOOST_CHECK(!env1.defined_locally("c"s));

    BOOST_CHECK(env2.defined("a"s));
    BOOST_CHECK(env2.defined("b"s));
    BOOST_CHECK(env2.defined("c"s));
    BOOST_CHECK(!env2.defined_locally("a"s));
    BOOST_CHECK(env2.defined_locally("b"s));
    BOOST_CHECK(env2.defined_locally("c"s));
}

BOOST_AUTO_TEST_SUITE_END()
