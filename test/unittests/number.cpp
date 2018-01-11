#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <sstream>

#include <nuschl/number.hpp>

BOOST_AUTO_TEST_SUITE(Number)
BOOST_AUTO_TEST_CASE(comparison) {
    nuschl::number a(2);
    nuschl::number b(2);
    nuschl::number c(3);
    nuschl::number d(0);

    BOOST_CHECK(a == b);
    BOOST_CHECK(!(a == c));
    BOOST_CHECK(a != c);
    BOOST_CHECK(!(a == c));
    BOOST_CHECK(a < c);
    BOOST_CHECK(d < a);
    BOOST_CHECK(c > a);
    BOOST_CHECK(b > d);
}

BOOST_AUTO_TEST_CASE(cast) {
    nuschl::number a(2);
    BOOST_CHECK_EQUAL(a.get_value(), 2);
}

BOOST_AUTO_TEST_CASE(negate) {
    nuschl::number a(2);
    BOOST_CHECK_EQUAL((-a).get_value(), -2);
}

BOOST_AUTO_TEST_CASE(addition) {
    nuschl::number a(2);
    nuschl::number b(2);
    nuschl::number c(4);
    nuschl::number d(0);

    BOOST_CHECK_EQUAL(a + b, c);
    BOOST_CHECK_EQUAL(a + d, b);
}

BOOST_AUTO_TEST_CASE(subtraction) {
    nuschl::number a(2);
    nuschl::number b(2);
    nuschl::number c(4);
    nuschl::number d(0);

    BOOST_CHECK_EQUAL(a - a, d);
    BOOST_CHECK_EQUAL(a - b, d);
    BOOST_CHECK_EQUAL(c - b, b);
}

BOOST_AUTO_TEST_CASE(multiplication) {
    nuschl::number a(2);
    nuschl::number b(2);
    nuschl::number c(4);
    nuschl::number d(0);

    BOOST_CHECK_EQUAL(a * b, c);
    BOOST_CHECK_EQUAL(a * d, d);
}

BOOST_AUTO_TEST_CASE(division) {
    nuschl::number a(2);
    nuschl::number b(2);
    nuschl::number c(4);
    nuschl::number d(0);

    BOOST_CHECK_EQUAL(c / b, a);
    BOOST_CHECK_EQUAL(d / c, d);
}

BOOST_AUTO_TEST_CASE(ostream) {
    std::stringstream ss;
    nuschl::number c(4);
    ss << c;
    BOOST_CHECK_EQUAL(ss.str(), "4");
}

BOOST_AUTO_TEST_SUITE_END()
