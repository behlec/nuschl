#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/s_exp.hpp>

#include <sstream>

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(S_Exp)
BOOST_AUTO_TEST_CASE(nil) {
    BOOST_CHECK(s_exp().is_nil());
    BOOST_CHECK(s_exp(s_exp::nil, s_exp::nil).is_nil());
    BOOST_CHECK(s_exp(nullptr, nullptr).is_nil());
}

BOOST_AUTO_TEST_CASE(create_from_atom) {
    const atom_ptr a = std::make_shared<atom>("hello");
    s_exp e(a);

    BOOST_REQUIRE(e.is_atom());
    BOOST_REQUIRE(e.get_atom()->is_symbol());
    BOOST_CHECK_EQUAL(e.get_atom()->get_symbol().get_value(), "hello");

    BOOST_CHECK(!e.is_cell());
    BOOST_CHECK(!e.is_primitive());
    BOOST_CHECK(!e.is_lambda());
}

BOOST_AUTO_TEST_CASE(create_list) {
    const atom_ptr a1 = std::make_shared<atom>(number{1});
    const atom_ptr a2 = std::make_shared<atom>(number{2});
    const atom_ptr a3 = std::make_shared<atom>(number{3});

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

BOOST_AUTO_TEST_CASE(ostream_nil) {
    std::stringstream ss;
    s_exp e;
    ss << &e;
    BOOST_CHECK_EQUAL(ss.str(), "nil");
}

BOOST_AUTO_TEST_CASE(ostream_atom) {
    std::stringstream ss;
    const atom_ptr a = std::make_shared<atom>("hello");
    s_exp e(a);
    ss << &e;
    BOOST_CHECK_EQUAL(ss.str(), "hello");
    ss.str("");

    const atom_ptr b = std::make_shared<atom>(number{1});
    e = s_exp(b);
    ss << &e;
    BOOST_CHECK_EQUAL(ss.str(), "1");
}

BOOST_AUTO_TEST_CASE(ostream_list) {
    std::stringstream ss;

    const atom_ptr a1 = std::make_shared<atom>(number{1});
    const atom_ptr a2 = std::make_shared<atom>(number{2});
    const atom_ptr a3 = std::make_shared<atom>(number{3});

    s_exp e3(a3);
    s_exp e2(a2);
    s_exp e1(a1);
    s_exp l3(&e3, s_exp::nil);
    s_exp l2(&e2, &l3);
    s_exp l(&e1, &l2);

    ss << &l;
    BOOST_CHECK_EQUAL(ss.str(), "(1, (2, (3, nil)))");
}

BOOST_AUTO_TEST_SUITE_END()
