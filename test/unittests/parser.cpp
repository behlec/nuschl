#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/parsing/parser.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <nuschl/unittests/string_to_s_exp.hpp>
#include <nuschl/unittests/parsestring.hpp>

#include <sstream>

using namespace std::string_literals;

namespace bdata = boost::unit_test::data;
namespace tt = boost::test_tools;

using nuschl::testing::parse_string;

BOOST_AUTO_TEST_SUITE(Parser)
BOOST_AUTO_TEST_CASE(Symbol) {
    parse_string v("foo");
    auto a = v();
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    BOOST_REQUIRE(a->car()->is_atom());
    BOOST_REQUIRE(a->car()->get_atom()->is_symbol());
    BOOST_CHECK(a->car()->get_atom()->get_symbol() == "foo");
}

BOOST_AUTO_TEST_CASE(Number) {
    parse_string v("23");
    auto a = v();
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    BOOST_REQUIRE(a->car()->is_atom());
    BOOST_REQUIRE(a->car()->get_atom()->is_number());
    BOOST_CHECK_EQUAL(a->car()->get_atom()->get_number(), nuschl::number(23));
}

BOOST_AUTO_TEST_CASE(List) {
    parse_string v("1 2 3");
    auto a = v();
    for (int i = 1; i < 4; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK_EQUAL(a->car()->get_atom()->get_number(),
                          nuschl::number(i));
        a = a->cdr();
    }
}

BOOST_AUTO_TEST_CASE(List2) {
    parse_string v("(1 2 3)");
    auto a = v();
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    a = a->car();
    for (int i = 1; i < 4; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK_EQUAL(a->car()->get_atom()->get_number(),
                          nuschl::number(i));
        a = a->cdr();
    }
    BOOST_CHECK(nuschl::is_empty_cell(a));
}
BOOST_AUTO_TEST_CASE(List3) {
    parse_string v("(1 2) (3 4)");
    auto l = v();
    BOOST_REQUIRE(l);
    BOOST_REQUIRE(l->is_cell());
    auto a = l->car();
    for (int i = 1; i < 3; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK_EQUAL(a->car()->get_atom()->get_number(),
                          nuschl::number(i));
        a = a->cdr();
    }
    BOOST_CHECK(is_empty_cell(a));
    a = l->cdr();
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    a = a->car();
    for (int i = 3; i < 5; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK_EQUAL(a->car()->get_atom()->get_number(),
                          nuschl::number(i));
        a = a->cdr();
    }
    BOOST_CHECK(is_empty_cell(a));
}

BOOST_AUTO_TEST_SUITE_END()

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(ParserData)

memory::s_exp_pool pool;

std::vector<testing::string_to_s_exp> examples = {
    {""s, pool.create(s_exp::nil, s_exp::nil)},
    {"nil"s,
     pool.create(pool.create_sym("nil"), pool.create(s_exp::nil, s_exp::nil))},
    {"()"s, pool.create(pool.create(s_exp::nil, s_exp::nil),
                        pool.create(s_exp::nil, s_exp::nil))},
    {"1"s, pool.create(pool.create(make_atom(number{1})),
                       pool.create(s_exp::nil, s_exp::nil))},
    {"1 2"s, pool.create(pool.create_num(number{1}),
                         pool.create(pool.create_num(number{2}),
                                     pool.create(s_exp::nil, s_exp::nil)))},
    {"(1)"s, pool.create(pool.create(pool.create(make_atom(number{1})),
                                     pool.create(s_exp::nil, s_exp::nil)),
                         pool.create(s_exp::nil, s_exp::nil))}};

BOOST_DATA_TEST_CASE(Parsing, bdata::make(examples), example) {
    nuschl::parsing::parser p(example.input, pool);
    auto a = p.parse().ast;
    BOOST_REQUIRE(a);
    BOOST_CHECK_EQUAL(*example.expected, *a);
}

BOOST_AUTO_TEST_SUITE_END()
