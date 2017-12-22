#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/util/primitives_impl.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

#include <nuschl/unittests/helper.hpp>

#include <sstream>

using namespace nuschl;

BOOST_AUTO_TEST_SUITE(Primitives)
memory::s_exp_pool pool;
BOOST_AUTO_TEST_CASE(All_numbers) {
    auto n1 = make_atom(number{1});
    auto n2 = make_atom(number{2});
    auto s1 = make_atom(symbol("foo"));

    s_exp e1(n1);
    s_exp e2(n2);
    s_exp e3(s1);

    std::vector<s_exp_ptr> l1 = {&e1};
    std::vector<s_exp_ptr> l2 = {&e1, &e2};
    std::vector<s_exp_ptr> l3 = {&e1, &e3, &e2};

    BOOST_CHECK_NO_THROW(nuschl::primitive_impl::all_numbers(l1));
    BOOST_CHECK_NO_THROW(nuschl::primitive_impl::all_numbers(l2));
    BOOST_CHECK_THROW(nuschl::primitive_impl::all_numbers(l3),
                      nuschl::eval_argument_error);
}

BOOST_AUTO_TEST_CASE(exact_args) {
    auto n1 = make_atom(number{1});
    auto n2 = make_atom(number{2});
    auto s1 = make_atom(symbol("foo"));

    s_exp e1(n1);
    s_exp e2(n2);
    s_exp e3(s1);

    std::vector<s_exp_ptr> l1 = {&e1};
    std::vector<s_exp_ptr> l2 = {&e1, &e3};
    std::vector<s_exp_ptr> l3 = {&e1, &e3, &e2};

    primitive_impl::argument_checker two_args = primitive_impl::exact_n_args(2);
    BOOST_CHECK_THROW(two_args(l1), nuschl::eval_argument_error);
    BOOST_CHECK_NO_THROW(two_args(l2));
    BOOST_CHECK_THROW(two_args(l3), nuschl::eval_argument_error);
}

BOOST_AUTO_TEST_CASE(least_args) {
    auto n1 = make_atom(number{1});
    auto n2 = make_atom(number{2});
    auto s1 = make_atom(symbol("foo"));

    s_exp e1(n1);
    s_exp e2(n2);
    s_exp e3(s1);

    std::vector<s_exp_ptr> l1 = {&e1};
    std::vector<s_exp_ptr> l2 = {&e1, &e3};
    std::vector<s_exp_ptr> l3 = {&e1, &e3, &e2};

    primitive_impl::argument_checker ltwo_args =
        primitive_impl::least_n_args(2);

    BOOST_CHECK_THROW(ltwo_args(l1), nuschl::eval_argument_error);
    BOOST_CHECK_NO_THROW(ltwo_args(l2));
    BOOST_CHECK_NO_THROW(ltwo_args(l3));
}

BOOST_AUTO_TEST_CASE(combine_checker) {
    auto n1 = make_atom(number{1});
    auto n2 = make_atom(number{2});
    auto s1 = make_atom(symbol("foo"));

    s_exp e1(n1);
    s_exp e2(n2);
    s_exp e3(s1);

    std::vector<s_exp_ptr> l1 = {&e1};
    std::vector<s_exp_ptr> l2 = {&e1, &e2};
    std::vector<s_exp_ptr> l3 = {&e1, &e2, &e3};

    primitive_impl::argument_checker ltwo_args =
        primitive_impl::least_n_args(2);

    primitive_impl::argument_checker c =
        ltwo_args && nuschl::primitive_impl::all_numbers;

    BOOST_CHECK_THROW(c(l1), nuschl::eval_argument_error);
    BOOST_CHECK_NO_THROW(c(l2));
    BOOST_CHECK_THROW(c(l3), nuschl::eval_argument_error);
}

BOOST_AUTO_TEST_CASE(primitive_builder) {

    nuschl::primitive_impl::primitivebuilder id(
        "id",
        [](const std::vector<s_exp_ptr> &arguments, memory::s_exp_pool *) {
            assert(arguments.size() == 1);
            return arguments[0];
        },
        nuschl::primitive_impl::exact_n_args(1));

    auto n1 = make_atom(number{1});
    auto n2 = make_atom(number{1});
    s_exp e1(n1);
    s_exp e2(n2);
    std::vector<s_exp_ptr> l1 = {&e1};
    std::vector<s_exp_ptr> l2 = {&e1, &e1};
    std::vector<s_exp_ptr> l3 = {&e1, &e2};
    std::vector<s_exp_ptr> l4 = {&e2};

    BOOST_CHECK(*id.execute(l1, &pool) == e1);
    BOOST_CHECK(*id.execute(l4, &pool) == e2);
    BOOST_CHECK_THROW(id.execute(l2, &pool), nuschl::eval_argument_error);
    BOOST_CHECK_THROW(id.execute(l3, &pool), nuschl::eval_argument_error);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(ExtractNumbers)
memory::s_exp_pool pool;
BOOST_AUTO_TEST_CASE(empty) {
    std::vector<s_exp_ptr> args;
    std::vector<number> res;
    primitive_impl::to_numbers(args.begin(), args.end(),
                               std::back_inserter(res));

    BOOST_CHECK(res.empty());
}

BOOST_AUTO_TEST_CASE(simple) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    std::vector<number> res;
    BOOST_REQUIRE_NO_THROW(primitive_impl::all_numbers(args));
    primitive_impl::to_numbers(args.begin(), args.end(),
                               std::back_inserter(res));

    BOOST_REQUIRE_EQUAL(res.size(), 1);
    BOOST_CHECK_EQUAL(res[0], number{1});
}

BOOST_AUTO_TEST_CASE(simple2) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(2, &pool));
    args.push_back(make_number(3, &pool));
    std::vector<number> res;
    BOOST_REQUIRE_NO_THROW(primitive_impl::all_numbers(args));
    primitive_impl::to_numbers(args.begin(), args.end(),
                               std::back_inserter(res));

    BOOST_REQUIRE_EQUAL(res.size(), 3);
    BOOST_CHECK_EQUAL(res[0], number{1});
    BOOST_CHECK_EQUAL(res[1], number{2});
    BOOST_CHECK_EQUAL(res[2], number{3});
}

BOOST_AUTO_TEST_SUITE_END()
