#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/lexer.hpp>

namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(Token)
BOOST_AUTO_TEST_CASE(create) {
    std::string s("aa");
    nuschl::parsing::token t0(nuschl::parsing::token::tokens::atom, s.begin(),
                              s.begin());
    nuschl::parsing::token t1(nuschl::parsing::token::tokens::atom, s.begin(),
                              s.begin() + 1);
    nuschl::parsing::token t2(nuschl::parsing::token::tokens::atom, s.begin(),
                              s.begin() + 2);
    BOOST_CHECK_EQUAL(t0.value(), "");
    BOOST_CHECK_EQUAL(t1.value(), "a");
    BOOST_CHECK_EQUAL(t2.value(), "aa");
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TokenIterator)

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
BOOST_DATA_TEST_CASE(AAtom, bdata::make(aatom), input) {
    nuschl::parsing::lexer l(input);
    BOOST_REQUIRE(l.begin() != l.end());
    BOOST_CHECK_EQUAL((*l.begin()).value(), "aa");
    BOOST_CHECK_EQUAL(l.begin()->value(), "aa");
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
BOOST_AUTO_TEST_CASE(Increment) {
    std::string input("() ( ) ab ac ( ad)ae");
    nuschl::parsing::lexer l(input);
    BOOST_CHECK(l.begin() != l.end());
    auto it = l.begin();
    BOOST_CHECK_EQUAL(it->value(), "(");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), ")");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "(");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), ")");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ab");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ac");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "(");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ad");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), ")");
    ++it;
    BOOST_CHECK_EQUAL(it->value(), "ae");
    BOOST_CHECK(it != l.end());
    ++it;
    BOOST_CHECK(it == l.end());
}
BOOST_AUTO_TEST_CASE(PostIncrement) {
    std::string input("() ( ) ab ac ( ad)ae");
    nuschl::parsing::lexer l(input);
    BOOST_CHECK(l.begin() != l.end());
    auto it = l.begin();
    BOOST_CHECK_EQUAL(it++->value(), "(");
    BOOST_CHECK_EQUAL(it++->value(), ")");
    BOOST_CHECK_EQUAL(it++->value(), "(");
    BOOST_CHECK_EQUAL(it++->value(), ")");
    BOOST_CHECK_EQUAL(it++->value(), "ab");
    BOOST_CHECK_EQUAL(it++->value(), "ac");
    BOOST_CHECK_EQUAL(it++->value(), "(");
    BOOST_CHECK_EQUAL(it++->value(), "ad");
    BOOST_CHECK_EQUAL(it++->value(), ")");
    BOOST_CHECK_EQUAL(it->value(), "ae");
    BOOST_CHECK(it++ != l.end());
    BOOST_CHECK(it == l.end());
}
BOOST_AUTO_TEST_SUITE_END()
