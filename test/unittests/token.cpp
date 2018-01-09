#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/parsing/lexer.hpp>

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
