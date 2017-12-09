#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/atom.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(Atom)
nuschl::symbol s("hallo");
nuschl::symbol t("hallo");
nuschl::symbol u("hello");
nuschl::number n(23);
nuschl::number m(23);
nuschl::number o(42);

nuschl::atom a(s);
nuschl::atom b(std::move(t));
nuschl::atom c(u);
nuschl::atom d(n);
nuschl::atom e(m);
nuschl::atom f(o);

BOOST_AUTO_TEST_CASE(creation) {
    BOOST_REQUIRE(a.is_symbol());
    BOOST_REQUIRE(b.is_symbol());
    BOOST_REQUIRE(c.is_symbol());
    BOOST_CHECK_EQUAL(a.get_symbol().get_value(), "hallo");
    BOOST_CHECK_EQUAL(b.get_symbol().get_value(), "hallo");
    BOOST_CHECK_EQUAL(c.get_symbol().get_value(), "hello");

    BOOST_REQUIRE(d.is_number());
    BOOST_REQUIRE(e.is_number());
    BOOST_REQUIRE(f.is_number());
    BOOST_CHECK_EQUAL(d.get_number().get_value(), 23);
    BOOST_CHECK_EQUAL(e.get_number().get_value(), 23);
    BOOST_CHECK_EQUAL(f.get_number().get_value(), 42);
}

BOOST_AUTO_TEST_CASE(comparison) {
    BOOST_CHECK_EQUAL(a, a);
    BOOST_CHECK_EQUAL(a, b);
    BOOST_CHECK_EQUAL(b, a);

    BOOST_CHECK(a != c);

    BOOST_CHECK_EQUAL(d, d);
    BOOST_CHECK_EQUAL(d, e);
    BOOST_CHECK_EQUAL(e, d);

    BOOST_CHECK(d != f);
    BOOST_CHECK(a != d);
}

BOOST_AUTO_TEST_CASE(ostream) {
    std::stringstream ss;
    ss << a;
    BOOST_CHECK_EQUAL(ss.str(), "hallo");
    ss.str("");
    ss << d;
    BOOST_CHECK_EQUAL(ss.str(), "23");
}

BOOST_AUTO_TEST_SUITE_END()