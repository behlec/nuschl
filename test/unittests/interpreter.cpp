#define BOOST_TEST_DYN_LINK

// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/default_env.hpp>
#include <nuschl/interpreter.hpp>
#include <nuschl/memory/s_exp_pool.hpp>
#include <nuschl/parsing/parser.hpp>

#include <nuschl/unittests/string_to_s_exp.hpp>
#include <nuschl/unittests/string_to_string.hpp>

#include <nuschl/exceptions.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

using namespace std::string_literals;

namespace bdata = boost::unit_test::data;
namespace tt = boost::test_tools;

BOOST_AUTO_TEST_SUITE(TestInterpreter)

nuschl::memory::s_exp_pool pool;

std::vector<nuschl::testing::string_to_s_exp> examples = {
    {"(define a 10) a"s, pool.create(make_atom(nuschl::number{10}))},
    {"5"s, pool.create(make_atom(nuschl::number{5}))},
    {"(if (eq 0 0) 1 2)"s, pool.create(make_atom(nuschl::number{1}))},
    {"(if (eq 0 0) 1)"s, pool.create(make_atom(nuschl::number{1}))},
    {"(if (eq 0 1) 1 2)"s, pool.create(make_atom(nuschl::number{2}))},
    {"(+ 1 2 3)"s, pool.create(make_atom(nuschl::number{6}))},
    {"(quote 3)"s, pool.create(make_atom(nuschl::number{3}))},
    {"(quote x)"s, pool.create(make_atom(nuschl::symbol{"x"}))},
    {"(begin 1 4 3)"s, pool.create(make_atom(nuschl::number{3}))},
    {"()"s, pool.create(nuschl::s_exp::nil, nuschl::s_exp::nil)},
    {"(let ((a 10)) a)"s, pool.create(make_atom(nuschl::number{10}))},
    {"(let ((a 10)) 3 a)"s, pool.create(make_atom(nuschl::number{10}))},
    {"((lambda (x) (* 2 x)) 2)"s, pool.create_atom(nuschl::number{4})},
    {"(define length (lambda (l) (if (eq nil l) 0 (+ 1 (length (cdr l)))))) (length (list 1 2 3))"s,
     pool.create(make_atom(nuschl::number{3}))}};

BOOST_DATA_TEST_CASE(Data, bdata::make(examples), example) {
    std::string code = example.input;
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(*example.expected, *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(Tprim) {
    std::string code = "+";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(interp.proc(pres.ast)->is_primitive());
}

BOOST_AUTO_TEST_CASE(Tnil) {
    std::string code = "nil";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(nuschl::s_exp::nil == interp.proc(pres.ast));
    BOOST_CHECK(*nuschl::s_exp::nil == *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(Tlambda) {
    std::string code = "(lambda (x) x)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK(interp.proc(pres.ast)->is_lambda());
}

BOOST_AUTO_TEST_CASE(Eqnil) {
    std::string code = "(eq nil nil)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(nuschl::s_exp::tru, interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(Eqnilel) {
    std::string code = "(eq nil (list))";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(nuschl::s_exp::tru, interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(Eqnilel2) {
    std::string code = "(eq nil ())";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(nuschl::s_exp::tru, interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(Quote) {
    std::string code = "(quote 5)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(*pool.create_atom(nuschl::number{5}),
                      *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(UnboundVariable) {
    std::string code = "x";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EXCEPTION(interp.proc(pres.ast), nuschl::eval_error,
                          [](const nuschl::eval_error &e) {
                              return "Unbound variable: x"s == e.what();
                          });
}

BOOST_AUTO_TEST_CASE(WrongDefine) {
    std::string code = "(define 3 4)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EXCEPTION(interp.proc(pres.ast), nuschl::eval_error,
                          [](const nuschl::eval_error &e) {
                              return "Expected symbol as first argument"s ==
                                     e.what();
                          });
}

BOOST_AUTO_TEST_CASE(GoodLet) {
    std::string code = "(let ((a 4)(b 2)) a)";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EQUAL(*pool.create_atom(nuschl::number{4}),
                      *interp.proc(pres.ast));
}

BOOST_AUTO_TEST_CASE(WrongPrimitiveInvocation) {
    std::string code = "(+ (quote x))";
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EXCEPTION(interp.proc(pres.ast), nuschl::eval_error,
                          [](const nuschl::eval_error &e) {
                              return "+ expects only numbers as arguments."s ==
                                     e.what();
                          });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestLet)

nuschl::memory::s_exp_pool pool;

std::vector<nuschl::testing::string_to_string> examples = {
    {"(let)"s,
     "Let requires one argument with the list of pairs and the body"s},
    {"(let 2)"s,
     "Let requires one argument with the list of pairs and the body"s},
    {"(let ((3 4)) a)"s, "Expected symbol as first part of pair, got 3"s},
    {"(let 3 4)"s, "Let requires list as first arguments"s},
    {"(let ((a 3 4)) a)"s, "Let requires list of pairs as argument"s}};

BOOST_DATA_TEST_CASE(WrongLambda, bdata::make(examples), example) {
    std::string code = example.input;
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EXCEPTION(interp.proc(pres.ast), nuschl::eval_error,
                          [&example](const nuschl::eval_error &e) {
                              return example.expected == e.what();
                          });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(TestLambda)

nuschl::memory::s_exp_pool pool;

std::vector<nuschl::testing::string_to_string> examples = {
    {"(lambda 3)"s, "Expect at least two arguments to lambda"s},
    {"(lambda (3) 3)"s,
     "Expected list of symbols as first argument to lambda"s},
    {"((lambda (x y) (+ x 1)) 2)"s, "Too few arguments for lambda"s},
    {"(lambda 3 4)"s, "Expected list as first argument to lambda"s},
    {"((lambda (x) 3) 1 2)"s, "Too many arguments for lambda"s}};

BOOST_DATA_TEST_CASE(WrongLambda, bdata::make(examples), example) {
    std::string code = example.input;
    nuschl::parsing::parser p(code, pool);
    auto pres = p.parse();
    nuschl::interpreter interp(nuschl::default_env.copy(), &pool);
    BOOST_CHECK_EXCEPTION(interp.proc(pres.ast), nuschl::eval_error,
                          [&example](const nuschl::eval_error &e) {
                              return example.expected == e.what();
                          });
}

BOOST_AUTO_TEST_SUITE_END()
