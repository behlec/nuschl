#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>

#include <nuschl/default_env.hpp>
#include <nuschl/interpreter.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/parsing/parser.hpp>

using namespace std::string_literals;

// namespace bdata = boost::unit_test::data;
// namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(testInterpreter)
nuschl::memory::s_exp_pool pool;
BOOST_AUTO_TEST_CASE(tsymbol) {
    std::string code = "(define a 10) a";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{10})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tnum) {
    std::string code = "5";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{5})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tif) {
    std::string code = "(if (eq 0 0) 1 2)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{1})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tifna) {
    std::string code = "(if (eq 0 0) 1)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{1})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tifa) {
    std::string code = "(if (eq 0 1) 1 2)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{2})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tprim) {
    std::string code = "+";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_TEST(interp.proc(pres.ast)->is_primitive());
}

BOOST_AUTO_TEST_CASE(tsum) {
    std::string code = "(+ 1 2 3)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{6})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tnil) {
    std::string code = "nil";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp::nil == interp.proc(pres.ast));
    BOOST_CHECK(*nuschl::s_exp::nil == *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(EmptyList) {
    std::string code = "()";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(*pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tlet) {
    std::string code = "(let ((a 10)) a)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{10})) ==
                *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tlambda) {
    std::string code = "(lambda (x) x)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(interp.proc(pres.ast)->is_lambda());
}

BOOST_AUTO_TEST_CASE(tlambdasim) {
    std::string code = "((lambda (x) (* 2 x)) 2)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(*interp.proc(pres.ast),
                      *pool.create_atom(nuschl::number{4}));
}

BOOST_AUTO_TEST_CASE(eqnil) {
    std::string code = "(eq nil nil)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(nuschl::s_exp::tru, interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(eqnilel) {
    std::string code = "(eq nil (list))";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(nuschl::s_exp::tru, interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(eqnilel2) {
    std::string code = "(eq nil ())";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(nuschl::s_exp::tru, interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(tlambda2) {
    std::string code = "(define length (lambda (l) (if (eq nil l) 0 (+ 1 "
                       "(length (cdr l)))))) (length (list 1 2 3))";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    auto res = interp.proc(pres.ast);
    BOOST_CHECK(nuschl::s_exp(make_atom(nuschl::number{3})) == *res);
}
BOOST_AUTO_TEST_SUITE_END()
