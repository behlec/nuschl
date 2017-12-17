#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/parsing/lexer.hpp>

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(ReverseTokenIterator)

namespace {
std::string notokens[] = {"", " "};
std::string parens[] = {"()",    "( )",  " ()",  " ( )",
                        " ( ) ", "( ) ", " () ", "() "};
std::string atom[] = {"a", " a", " a ", "a "};
std::string aatom[] = {"aa", " aa", " aa ", "aa "};
std::string testfin[] = {"aa", "aa(", "aa)", "aa "};
std::string lparen[] = {"(", "( ", " ( "};
std::string rparen[] = {")", ") ", " ) "};
}

BOOST_DATA_TEST_CASE(NoTokens, bdata::make(notokens), input) {
    nuschl::parsing::lexer l(input);
    BOOST_CHECK(l.rbegin() == l.rend());
}
BOOST_DATA_TEST_CASE(Atom, bdata::make(atom), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.rbegin() != l.rend());
    BOOST_CHECK_EQUAL((*l.rbegin()).value(), "a");
    BOOST_CHECK_EQUAL(l.rbegin()->value(), "a");
}
BOOST_DATA_TEST_CASE(LParen, bdata::make(lparen), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.rbegin() != l.rend());
    BOOST_CHECK_EQUAL((*l.rbegin()).value(), "(");
    BOOST_CHECK_EQUAL(l.rbegin()->value(), "(");
}
BOOST_DATA_TEST_CASE(RParen, bdata::make(rparen), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.rbegin() != l.rend());
    BOOST_CHECK_EQUAL((*l.rbegin()).value(), ")");
    BOOST_CHECK_EQUAL(l.rbegin()->value(), ")");
}
BOOST_DATA_TEST_CASE(AAtom, bdata::make(aatom), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.rbegin() != l.rend());
    BOOST_CHECK_EQUAL((*l.rbegin()).value(), "aa");
    BOOST_CHECK_EQUAL(l.rbegin()->value(), "aa");
}
BOOST_DATA_TEST_CASE(Parens, bdata::make(parens), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.rbegin() != l.rend());
    auto t = *l.rbegin();
    BOOST_CHECK(t.is_rparan());
    BOOST_CHECK_EQUAL(t.value(), ")");
    BOOST_CHECK(l.rbegin()->is_rparan());
    BOOST_CHECK_EQUAL(l.rbegin()->value(), ")");
}
BOOST_AUTO_TEST_CASE(Increment) {
    std::string input("() ( ) ab ac ( ad)ae");
    nuschl::parsing::lexer l(input);
    BOOST_CHECK(l.rbegin() != l.rend());
    auto it = l.rbegin();
    BOOST_CHECK_EQUAL(it->value(), "ae");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), ")");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ad");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "(");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ac");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ab");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), ")");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "(");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), ")");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "(");
    BOOST_CHECK(it != l.rend());
    ++it;
    BOOST_CHECK(it == l.rend());
}
BOOST_AUTO_TEST_CASE(PostIncrement) {
    std::string input("() ( ) ab ac ( ad)ae");
    nuschl::parsing::lexer l(input);
    BOOST_CHECK(l.rbegin() != l.rend());
    auto it = l.rbegin();
    BOOST_CHECK_EQUAL(it++->value(), "ae");
    BOOST_CHECK_EQUAL(it++->value(), ")");
    BOOST_CHECK_EQUAL(it++->value(), "ad");
    BOOST_CHECK_EQUAL(it++->value(), "(");
    BOOST_CHECK_EQUAL(it++->value(), "ac");
    BOOST_CHECK_EQUAL(it++->value(), "ab");
    BOOST_CHECK_EQUAL(it++->value(), ")");
    BOOST_CHECK_EQUAL(it++->value(), "(");
    BOOST_CHECK_EQUAL(it++->value(), ")");
    BOOST_CHECK_EQUAL(it->value(), "(");
    BOOST_CHECK(it++ != l.rend());
    BOOST_CHECK(it == l.rend());
}
BOOST_AUTO_TEST_SUITE_END()
