#define BOOST_CHECK_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
// clang-format on

#include <nuschl/builtins.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/util/s_exp_helpers.hpp>
//#include <nuschl/default_env.hpp>
#include <nuschl/parsing/parser.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

#include <nuschl/unittests/helper.hpp>

#include <memory>

using namespace std::string_literals;

BOOST_AUTO_TEST_SUITE(testS_Exp_Helpers)

nuschl::memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(tisfalse) {
    auto empty_list = pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil);
    BOOST_CHECK(!nuschl::to_bool(nuschl::s_exp::nil));
    BOOST_CHECK(nuschl::to_bool(empty_list));
    BOOST_CHECK(!nuschl::to_bool(nuschl::s_exp::fals));
    BOOST_CHECK(nuschl::to_bool(nuschl::s_exp::tru));
    auto a = nuschl::make_number(0, &pool);
    BOOST_CHECK(nuschl::to_bool(a));
    auto l = pool.create(nuschl::make_number(0, &pool), nuschl::s_exp::nil);
    BOOST_CHECK(nuschl::to_bool(l));
}

BOOST_AUTO_TEST_CASE(tlist_size) {
    auto f = nuschl::s_exp::fals;
    auto l0 = pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil);
    BOOST_CHECK_EQUAL(0ul, nuschl::list_size(nuschl::s_exp::nil));
    BOOST_CHECK_EQUAL(0ul, nuschl::list_size(l0));
    auto l1 = pool.create(f, l0);
    auto l2 = pool.create(f, l1);
    auto l3 = pool.create(f, l2);
    BOOST_CHECK_EQUAL(1ul, nuschl::list_size(l1));
    BOOST_CHECK_EQUAL(2ul, nuschl::list_size(l2));
    BOOST_CHECK_EQUAL(3ul, nuschl::list_size(l3));
}

BOOST_AUTO_TEST_CASE(tlist_to_cont) {
    std::vector<const nuschl::s_exp *> res;
    std::string code = "1 2 3 4";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::list_to_cont(pres.ast, std::back_inserter(res));
    const nuschl::s_exp exp[] = {
        nuschl::s_exp(nuschl::make_atom(nuschl::number{1})),
        nuschl::s_exp(nuschl::make_atom(nuschl::number{2})),
        nuschl::s_exp(nuschl::make_atom(nuschl::number{3})),
        nuschl::s_exp(nuschl::make_atom(nuschl::number{4}))};
    BOOST_REQUIRE_EQUAL(4ul, res.size());
    BOOST_CHECK_EQUAL(exp[0], *res[0]);
    BOOST_CHECK_EQUAL(exp[1], *res[1]);
    BOOST_CHECK_EQUAL(exp[2], *res[2]);
    BOOST_CHECK_EQUAL(exp[3], *res[3]);
}

BOOST_AUTO_TEST_CASE(tlist_to_cont_empty) {
    std::vector<const nuschl::s_exp *> res;
    std::string code = "";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::list_to_cont(pres.ast, std::back_inserter(res));
    BOOST_CHECK_EQUAL(0ul, res.size());
    auto ast = pool.create(nuschl::make_number(1, &pool), nuschl::s_exp::nil);
    nuschl::list_to_cont(ast, std::back_inserter(res));
    BOOST_REQUIRE_EQUAL(1ul, res.size());
    BOOST_CHECK(is_number(res[0]));
}

BOOST_AUTO_TEST_CASE(tto_list) {
    std::vector<const nuschl::s_exp *> vec = {
        pool.create(make_atom(nuschl::number{1})),
        pool.create(make_atom(nuschl::number{2})),
        pool.create(make_atom(nuschl::number{3})),
        pool.create(make_atom(nuschl::number{4}))};
    auto res = nuschl::to_list(vec.begin(), vec.end(), &pool);

    std::string code = "1 2 3 4";
    nuschl::parsing::parser p(code, pool);
    auto exp = p.parse();
    BOOST_CHECK_EQUAL(*res, *exp.ast);
}

BOOST_AUTO_TEST_CASE(tto_list_empty) {
    std::vector<const nuschl::s_exp *> vec;
    auto res = nuschl::to_list(vec.begin(), vec.end(), &pool);

    auto e = pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil);

    BOOST_CHECK_EQUAL(*res, *e);
}

BOOST_AUTO_TEST_CASE(tconvert_cycle) {
    std::vector<const nuschl::s_exp *> vec = {
        pool.create(make_atom(nuschl::number{1})),
        pool.create(make_atom(nuschl::number{2})),
        pool.create(make_atom(nuschl::number{3})),
        pool.create(make_atom(nuschl::number{4}))};

    std::vector<const nuschl::s_exp *> res;
    nuschl::list_to_cont(nuschl::to_list(vec.begin(), vec.end(), &pool),
                         std::back_inserter(res));
    BOOST_CHECK(std::equal(vec.begin(), vec.end(), res.begin()));

    const nuschl::s_exp *l = pool.create(
        pool.create(nuschl::make_atom(nuschl::number{1})),
        pool.create(
            pool.create(make_atom(nuschl::number{2})),
            pool.create(pool.create(make_atom(nuschl::number{3})),
                        pool.create(pool.create(make_atom(nuschl::number{4})),
                                    pool.create(nuschl::s_exp::nil,
                                                nuschl::s_exp::nil)))));

    std::vector<const nuschl::s_exp *> tmp;
    nuschl::list_to_cont(l, std::back_inserter(tmp));
    auto r = nuschl::to_list(tmp.begin(), tmp.end(), &pool);
    BOOST_CHECK_EQUAL(*l, *r);
}

BOOST_AUTO_TEST_CASE(tisnil) { BOOST_CHECK(is_nil(nuschl::s_exp::nil)); }

BOOST_AUTO_TEST_CASE(tissymbol) {
    boost::scoped_ptr<const nuschl::s_exp> a(
        new nuschl::s_exp(nuschl::make_atom("s"s)));
    boost::scoped_ptr<const nuschl::s_exp> b(
        new nuschl::s_exp(nuschl::make_atom(nuschl::number{5})));
    BOOST_CHECK(is_symbol(a.get()));
    BOOST_CHECK(!is_symbol(nuschl::s_exp::nil));
    BOOST_CHECK(!is_symbol(b.get()));
}

BOOST_AUTO_TEST_CASE(tisprimitive) {
    const auto a = make_lambda({"a"s}, nuschl::s_exp::nil, nullptr);
    BOOST_CHECK(is_primitive(pool.create(&nuschl::primitives::plus)));
    BOOST_CHECK(!is_primitive(pool.create(a)));
}

BOOST_AUTO_TEST_CASE(tislambda) {
    BOOST_CHECK(is_lambda(
        pool.create(nuschl::make_lambda({"a"s}, nuschl::s_exp::nil, nullptr))));
    BOOST_CHECK(!is_lambda(pool.create(&nuschl::primitives::plus)));
}

BOOST_AUTO_TEST_CASE(IsEmptyCell) {
    BOOST_CHECK(
        is_empty_cell(pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil)));
    BOOST_CHECK(is_empty_cell(nuschl::s_exp::nil));
    BOOST_CHECK(!is_empty_cell(pool.create_atom(nuschl::number{1})));
    BOOST_CHECK(!is_empty_cell(pool.create_atom("a")));
    auto l = nuschl::make_lambda({}, nullptr, nullptr);
    BOOST_CHECK(!is_empty_cell(pool.create(l)));
}

BOOST_AUTO_TEST_CASE(For_List) {
    std::stringstream ss;
    auto l = pool.create(
        pool.create_atom(nuschl::number{1}),
        pool.create(pool.create_atom(nuschl::number{2}),
                    pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil)));
    for_list(l, [&ss](const nuschl::s_exp *l) { ss << l; });
    BOOST_CHECK_EQUAL(ss.str(), "12");
}

BOOST_AUTO_TEST_CASE(For_ListEmpty) {
    std::stringstream ss;
    auto l = pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil);
    for_list(l, [&ss](const nuschl::s_exp *l) { ss << l; });
    BOOST_CHECK_EQUAL(ss.str(), "");
}

BOOST_AUTO_TEST_SUITE_END()
