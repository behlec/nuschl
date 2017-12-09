#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/lexer.hpp>

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(Lexer)

namespace {
std::string notokens[] = {"", " "};
std::string parens[] = {"()",    "( )",  " ()",  " ( )",
                        " ( ) ", "( ) ", " () ", "() "};
std::string atom[] = {"a", " a", " a ", "a "};
std::string lparen[] = {"(", "( ", " ( "};
std::string rparen[] = {")", ") ", " ) "};
}

BOOST_DATA_TEST_CASE(NoTokens, bdata::make(notokens), input) {
    nuschl::parsing::lexer l(input);
    BOOST_CHECK(l.begin() == l.end());
}
BOOST_DATA_TEST_CASE(Atom, bdata::make(atom), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.begin() != l.end());
    BOOST_CHECK_EQUAL((*l.begin()).value(), "a");
    BOOST_CHECK_EQUAL(l.begin()->value(), "a");
}
BOOST_DATA_TEST_CASE(LParen, bdata::make(lparen), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.begin() != l.end());
    BOOST_CHECK_EQUAL((*l.begin()).value(), "(");
    BOOST_CHECK_EQUAL(l.begin()->value(), "(");
}
BOOST_DATA_TEST_CASE(RParen, bdata::make(rparen), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.begin() != l.end());
    BOOST_CHECK_EQUAL((*l.begin()).value(), ")");
    BOOST_CHECK_EQUAL(l.begin()->value(), ")");
}
BOOST_DATA_TEST_CASE(Parens, bdata::make(parens), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.begin() != l.end());
    auto t = *l.begin();
    BOOST_CHECK(t.is_lparan());
    BOOST_CHECK_EQUAL(t.value(), "(");
    BOOST_CHECK(l.begin()->is_lparan());
    BOOST_CHECK_EQUAL(l.begin()->value(), "(");
}
BOOST_AUTO_TEST_SUITE_END()
