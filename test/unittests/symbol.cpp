#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/symbol.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(Symbol)
BOOST_AUTO_TEST_CASE(creation) {
    nuschl::symbol s("hallo");
    std::string foo("foo");
    nuschl::symbol t(foo);
    nuschl::symbol u(std::string("tmp"));

    BOOST_CHECK_EQUAL(s.get_value(), "hallo");
    BOOST_CHECK_EQUAL(t.get_value(), "foo");
    BOOST_CHECK_EQUAL(u.get_value(), "tmp");
}

BOOST_AUTO_TEST_CASE(comparison) {
    nuschl::symbol s("hallo");
    nuschl::symbol t("hallo");
    nuschl::symbol u("hello");

    BOOST_CHECK(s == s);
    BOOST_CHECK(s == t);
    BOOST_CHECK(s != u);
}

BOOST_AUTO_TEST_CASE(ostream) {
    std::stringstream ss;
    nuschl::symbol s("hallo");
    ss << s;
    BOOST_CHECK_EQUAL(ss.str(), "hallo");
}

BOOST_AUTO_TEST_SUITE_END()
