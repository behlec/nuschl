#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <boost/scoped_ptr.hpp>

#include <nuschl/parsing/parser.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

using namespace std::string_literals;

namespace bdata = boost::unit_test::data;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(SimpleTests)
BOOST_AUTO_TEST_CASE(Symbol) {
    nuschl::memory::s_exp_pool pool;
    std::string code = "foo";
    nuschl::parsing::parser p(code, pool);
    auto a = p.parse().ast;
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    BOOST_REQUIRE(a->car()->is_atom());
    BOOST_REQUIRE(a->car()->get_atom()->is_symbol());
    BOOST_CHECK(a->car()->get_atom()->get_symbol() == "foo");
}

BOOST_AUTO_TEST_CASE(Number) {
    nuschl::memory::s_exp_pool pool;
    std::string code = "23";
    nuschl::parsing::parser p(code, pool);
    auto a = p.parse().ast;
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    BOOST_REQUIRE(a->car()->is_atom());
    BOOST_REQUIRE(a->car()->get_atom()->is_number());
    BOOST_CHECK(a->car()->get_atom()->get_number() == nuschl::number(23));
}

BOOST_AUTO_TEST_CASE(List) {
    nuschl::memory::s_exp_pool pool;
    std::string code = "1 2 3";
    nuschl::parsing::parser p(code, pool);
    auto a = p.parse().ast;
    for (int i = 1; i < 4; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK(a->car()->get_atom()->get_number() == nuschl::number(i));
        a = a->cdr();
    }
}

BOOST_AUTO_TEST_CASE(List2) {
    nuschl::memory::s_exp_pool pool;
    std::string code = "(1 2 3)";
    nuschl::parsing::parser p(code, pool);
    auto a = p.parse().ast;
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    a = a->car();
    for (int i = 1; i < 4; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK(a->car()->get_atom()->get_number() == nuschl::number(i));
        a = a->cdr();
    }
    BOOST_CHECK(a->is_nil());
}
BOOST_AUTO_TEST_CASE(List3) {
    nuschl::memory::s_exp_pool pool;
    std::string code = "(1 2) (3 4)";
    nuschl::parsing::parser p(code, pool);
    auto l = p.parse().ast;
    BOOST_REQUIRE(l);
    BOOST_REQUIRE(l->is_cell());
    auto a = l->car();
    for (int i = 1; i < 3; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK(a->car()->get_atom()->get_number() == nuschl::number(i));
        a = a->cdr();
    }
    BOOST_CHECK(a->is_nil());
    a = l->cdr();
    BOOST_REQUIRE(a);
    BOOST_REQUIRE(a->is_cell());
    a = a->car();
    for (int i = 3; i < 5; ++i) {
        BOOST_REQUIRE(a);
        BOOST_REQUIRE(a->is_cell());
        BOOST_REQUIRE(a->car()->is_atom());
        BOOST_CHECK(a->car()->get_atom()->get_number() == nuschl::number(i));
        a = a->cdr();
    }
    BOOST_CHECK(a->is_nil());
    BOOST_CHECK(l->cdr()->cdr()->is_nil());
}

BOOST_AUTO_TEST_SUITE_END()
