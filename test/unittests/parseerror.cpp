#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/parsing/parseerror.hpp>

#include <string>

using namespace std::string_literals;

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(ParseError)

BOOST_AUTO_TEST_CASE(ParseError) {
    std::string input("input");
    nuschl::parsing::parse_error e("reason", input.cbegin());
    BOOST_CHECK_EQUAL("reason"s, e.what());
    BOOST_CHECK(e.where() == input.cbegin());
}

BOOST_AUTO_TEST_CASE(Foo) {
    std::string foo("foo");
    auto e = new nuschl::parsing::parse_error("bar", foo.begin());
    delete e;
}

BOOST_AUTO_TEST_SUITE_END()
