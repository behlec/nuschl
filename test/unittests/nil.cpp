#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/parsing/parser.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

#include <nuschl/unittests/string_to_s_exp.hpp>
#include <nuschl/unittests/s_exp_to_string.hpp>
#include <nuschl/unittests/vector_printer.hpp>

#include <vector>
#include <sstream>

using namespace std::string_literals;

namespace bdata = boost::unit_test::data;
namespace tt = boost::test_tools;

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(Nil)

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Is_nil) {
    BOOST_CHECK(s_exp::nil->is_nil());
    BOOST_CHECK(!s_exp::nil->is_cell());
    auto e = pool.create(s_exp::nil, s_exp::nil);
    BOOST_CHECK(!e->is_nil());
    BOOST_CHECK(!s_exp::nil->is_cell());
}

std::vector<testing::s_exp_to_string> examples = {
    {s_exp::nil, "nil"},
    {pool.create(s_exp::nil, s_exp::nil), "(nil, nil)"},
    {pool.create(pool.create(make_atom(number{1})), s_exp::nil), "(1, nil)"},
    {pool.create(pool.create(make_atom(number{1})),
                 pool.create(s_exp::nil, s_exp::nil)),
     "(1, (nil, nil))"},
    {pool.create(pool.create(pool.create(make_atom(number{1})),
                             pool.create(s_exp::nil, s_exp::nil)),
                 pool.create(s_exp::nil, s_exp::nil)),
     "((1, (nil, nil)), (nil, nil))"},
};

BOOST_DATA_TEST_CASE(Parsing, bdata::make(examples), example) {
    std::stringstream ss;
    ss << *example.input;
    BOOST_CHECK_EQUAL(ss.str(), example.expected);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(NilParsing)

memory::s_exp_pool pool;

std::vector<testing::string_to_s_exp> examples = {
    {""s, pool.create(s_exp::nil, s_exp::nil)},
    {"nil"s,
     pool.create(pool.create_atom("nil"), pool.create(s_exp::nil, s_exp::nil))},
    {"()"s, pool.create(pool.create(s_exp::nil, s_exp::nil),
                        pool.create(s_exp::nil, s_exp::nil))},
    {"(nil)"s, pool.create(pool.create(pool.create_atom("nil"),
                                       pool.create(s_exp::nil, s_exp::nil)),
                           pool.create(s_exp::nil, s_exp::nil))},
    {"1"s, pool.create(pool.create(make_atom(number{1})),
                       pool.create(s_exp::nil, s_exp::nil))},
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
