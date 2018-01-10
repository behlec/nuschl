#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/memory/s_exp_pool.hpp>

#include <nuschl/unittests/unique.hpp>

#include <sstream>

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(S_Exp_Pool)

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(CreateAtom) {
    auto s = pool.create_atom(number{3});
    BOOST_REQUIRE(s->is_atom());
    BOOST_REQUIRE(s->get_atom()->is_number());
    BOOST_CHECK_EQUAL(s->get_atom()->get_number(), number{3});

    s = pool.create_atom(symbol{"ab"});
    BOOST_REQUIRE(s->is_atom());
    BOOST_REQUIRE(s->get_atom()->is_symbol());
    BOOST_CHECK_EQUAL(s->get_atom()->get_symbol(), symbol{"ab"});
}

BOOST_AUTO_TEST_CASE(Add) {
    const s_exp *s = new s_exp(make_atom(number{3}));
    pool.add(s);
}

BOOST_AUTO_TEST_SUITE_END()
