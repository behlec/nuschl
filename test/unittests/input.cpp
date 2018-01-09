#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/parsing/input.hpp>

#include <vector>

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(To_Line)

std::string pseudo_input = "foo\nbar\nbaz";

BOOST_AUTO_TEST_CASE(Sanity) {
    auto cur = pseudo_input.cbegin();
    auto e = pseudo_input.cend();
    cur = std::find(cur, e, '\n') + 1;
    BOOST_CHECK(cur != e);
    cur = std::find(cur, e, '\n') + 1;
    BOOST_CHECK(cur != e);
    cur = std::find(cur, e, '\n');
    BOOST_CHECK(cur == e);
}

BOOST_AUTO_TEST_CASE(To_Line) {
    auto res = nuschl::parsing::to_line(
        pseudo_input.cbegin(), pseudo_input.cbegin(), pseudo_input.cend());
    std::pair<std::size_t, std::size_t> exp(0, 0);
    std::string str = nuschl::parsing::to_line_string(
        pseudo_input.cbegin(), pseudo_input.cbegin(), pseudo_input.cend());
    BOOST_CHECK(res == exp);
    BOOST_CHECK(str == "0:0");
}

BOOST_AUTO_TEST_CASE(To_line2) {
    auto res = nuschl::parsing::to_line(
        pseudo_input.cbegin() + 1, pseudo_input.cbegin(), pseudo_input.cend());
    std::pair<std::size_t, std::size_t> exp(0, 1);
    std::string str = nuschl::parsing::to_line_string(
        pseudo_input.cbegin() + 1, pseudo_input.cbegin(), pseudo_input.cend());
    BOOST_CHECK(res == exp);
    BOOST_CHECK(str == "0:1");
}

BOOST_AUTO_TEST_CASE(To_Line3) {
    auto res = nuschl::parsing::to_line(
        pseudo_input.cbegin() + 4, pseudo_input.cbegin(), pseudo_input.cend());
    std::pair<std::size_t, std::size_t> exp(1, 0);
    std::string str = nuschl::parsing::to_line_string(
        pseudo_input.cbegin() + 4, pseudo_input.cbegin(), pseudo_input.cend());
    BOOST_CHECK(res == exp);
    BOOST_CHECK(str == "1:0");
}

BOOST_AUTO_TEST_CASE(To_Line4) {
    auto res = nuschl::parsing::to_line(
        pseudo_input.cbegin() + 5, pseudo_input.cbegin(), pseudo_input.cend());
    std::pair<std::size_t, std::size_t> exp(1, 1);
    std::string str = nuschl::parsing::to_line_string(
        pseudo_input.cbegin() + 5, pseudo_input.cbegin(), pseudo_input.cend());
    BOOST_CHECK(res == exp);
    BOOST_CHECK(str == "1:1");
}
BOOST_AUTO_TEST_SUITE_END()
