#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/parsing/lexer.hpp>

#include <sstream>

BOOST_AUTO_TEST_SUITE(Token)

BOOST_AUTO_TEST_CASE(CreateAtom) {
    std::string s("aa");
    nuschl::parsing::token t0(nuschl::parsing::token::tokens::atom, s.begin(),
                              s.begin());
    nuschl::parsing::token t1(nuschl::parsing::token::tokens::atom, s.begin(),
                              s.begin() + 1);
    nuschl::parsing::token t2(nuschl::parsing::token::tokens::atom, s.begin(),
                              s.begin() + 2);
    std::stringstream ss;
    ss.str("");
    BOOST_CHECK_EQUAL(t0.value(), "");
    BOOST_CHECK(t0.type() == nuschl::parsing::token::tokens::atom);
    BOOST_CHECK(t0.position() == s.begin());
    ss << t0;
    BOOST_CHECK_EQUAL(ss.str(), "");

    ss.str("");
    BOOST_CHECK_EQUAL(t1.value(), "a");
    BOOST_CHECK(t1.type() == nuschl::parsing::token::tokens::atom);
    BOOST_CHECK(t1.position() == s.begin());
    ss << t1;
    BOOST_CHECK_EQUAL(ss.str(), "a");

    ss.str("");
    BOOST_CHECK_EQUAL(t2.value(), "aa");
    BOOST_CHECK(t2.type() == nuschl::parsing::token::tokens::atom);
    BOOST_CHECK(t2.position() == s.begin());
    ss << t2;
    BOOST_CHECK_EQUAL(ss.str(), "aa");
}

BOOST_AUTO_TEST_CASE(CreateL) {
    std::string s("(");
    nuschl::parsing::token t0(nuschl::parsing::token::tokens::lparan, s.begin(),
                              s.begin() + 1);
    std::stringstream ss;
    ss.str("");
    BOOST_CHECK_EQUAL(t0.value(), "(");
    BOOST_CHECK(t0.type() == nuschl::parsing::token::tokens::lparan);
    BOOST_CHECK(t0.position() == s.begin());
    ss << t0;
    BOOST_CHECK_EQUAL(ss.str(), "(");
}

BOOST_AUTO_TEST_CASE(CreateR) {
    std::string s(")");
    nuschl::parsing::token t0(nuschl::parsing::token::tokens::rparan, s.begin(),
                              s.begin() + 1);
    std::stringstream ss;
    ss.str("");
    BOOST_CHECK_EQUAL(t0.value(), ")");
    BOOST_CHECK(t0.type() == nuschl::parsing::token::tokens::rparan);
    BOOST_CHECK(t0.position() == s.begin());
    ss << t0;
    BOOST_CHECK_EQUAL(ss.str(), ")");
}

BOOST_AUTO_TEST_SUITE_END()
