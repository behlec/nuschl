#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/s_exp.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

#include <nuschl/unittests/unique.hpp>

#include <sstream>

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(S_Exp)

BOOST_AUTO_TEST_CASE(Nil) {
    BOOST_CHECK(s_exp::nil->is_nil());
    BOOST_CHECK(!s_exp(s_exp::nil, s_exp::nil).is_nil());
    BOOST_CHECK(!s_exp(nullptr, nullptr).is_nil());
}

BOOST_AUTO_TEST_CASE(Create_from_atom) {
    const atom_ptr a = make_atom("hello");
    s_exp e(a);

    BOOST_REQUIRE(e.is_atom());
    BOOST_REQUIRE(e.get_atom()->is_symbol());
    BOOST_CHECK_EQUAL(e.get_atom()->get_symbol().get_value(), "hello");

    BOOST_CHECK(!e.is_cell());
    BOOST_CHECK(!e.is_primitive());
    BOOST_CHECK(!e.is_lambda());
    BOOST_CHECK(!e.is_nil());
}

BOOST_AUTO_TEST_CASE(Create_list) {
    const atom_ptr a1 = make_atom(number{1});
    const atom_ptr a2 = make_atom(number{2});
    const atom_ptr a3 = make_atom(number{3});

    s_exp e3(a3);
    s_exp e2(a2);
    s_exp e1(a1);
    s_exp l3(&e3, s_exp::nil);
    s_exp l2(&e2, &l3);
    s_exp l(&e1, &l2);

    BOOST_REQUIRE(l.is_cell());
    BOOST_REQUIRE(l2.is_cell());
    BOOST_REQUIRE(l3.is_cell());
    BOOST_CHECK_EQUAL(l.car(), &e1);
    BOOST_CHECK_EQUAL(l.cdr(), &l2);
    BOOST_CHECK_EQUAL(l2.car(), &e2);
    BOOST_CHECK_EQUAL(l2.cdr(), &l3);
    BOOST_CHECK_EQUAL(l3.car(), &e3);
    BOOST_CHECK_EQUAL(l3.cdr(), s_exp::nil);

    BOOST_CHECK(!l.is_atom());
    BOOST_CHECK(!l.is_primitive());
    BOOST_CHECK(!l.is_lambda());
}

BOOST_AUTO_TEST_CASE(Ostream_nil) {
    std::stringstream ss;
    ss << s_exp::nil;
    BOOST_CHECK_EQUAL(ss.str(), "nil");
}

BOOST_AUTO_TEST_CASE(Ostream_atom) {
    std::stringstream ss;
    const atom_ptr a = make_atom("hello");
    s_exp e(a);
    ss << &e;
    BOOST_CHECK_EQUAL(ss.str(), "hello");
    ss.str("");

    const atom_ptr b = make_atom(number{1});
    e = s_exp(b);
    ss << &e;
    BOOST_CHECK_EQUAL(ss.str(), "1");
}

BOOST_AUTO_TEST_CASE(Ostream_list) {
    std::stringstream ss;

    const atom_ptr a1 = make_atom(number{1});
    const atom_ptr a2 = make_atom(number{2});
    const atom_ptr a3 = make_atom(number{3});

    s_exp e3(a3);
    s_exp e2(a2);
    s_exp e1(a1);
    s_exp l3(&e3, s_exp::nil);
    s_exp l2(&e2, &l3);
    s_exp l(&e1, &l2);

    ss << &l;
    BOOST_CHECK_EQUAL(ss.str(), "(1, (2, (3, nil)))");
}

BOOST_AUTO_TEST_CASE(Equality_Nil) {
    s_exp s{s_exp::nil, s_exp::nil};
    BOOST_CHECK(*s_exp::nil != s);
    BOOST_CHECK_EQUAL(*s_exp::nil, *s_exp::nil);
}

BOOST_AUTO_TEST_CASE(Equality_Empty) {
    s_exp s{s_exp::nil, s_exp::nil};
    s_exp t{s_exp::nil, s_exp::nil};
    BOOST_CHECK_EQUAL(s, s);
    BOOST_CHECK_EQUAL(t, s);
}

BOOST_AUTO_TEST_CASE(Equality_Atom) {
    auto e1 = test::make_unique_num(1);
    auto e2 = test::make_unique_num(1);
    auto e3 = test::make_unique_num(2);

    BOOST_CHECK(e1 != e2);
    BOOST_CHECK_EQUAL(*e1.get(), *e1.get());
    BOOST_CHECK_EQUAL(*e1.get(), *e2.get());
    BOOST_CHECK_NE(*e1.get(), *e3.get());
}

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Equality_List) {
    auto e1 = pool.create(s_exp::nil, s_exp::nil);
    auto e2 = pool.create(s_exp::nil, s_exp::nil);
    auto a1 = pool.create(make_atom(number{1}));
    auto b1 = pool.create(make_atom(number{1}));
    auto a = pool.create(a1, e1);
    auto b = pool.create(b1, e2);
    auto c2 = pool.create(make_atom(number{2}));
    auto d2 = pool.create(make_atom(number{2}));
    auto c = pool.create(c2, a);
    auto d = pool.create(d2, b);
    BOOST_CHECK_EQUAL(*c, *d);
}

BOOST_AUTO_TEST_SUITE_END()
