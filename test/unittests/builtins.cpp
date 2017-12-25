#define BOOST_TEST_DYN_LINK
// clang-format off
#include <boost/test/unit_test.hpp>
#include <boost/test/data/test_case.hpp>
#include <boost/test/data/monomorphic.hpp>
// clang-format on

#include <nuschl/builtins.hpp>
#include <nuschl/s_exp.hpp>
#include <nuschl/memory/s_exp_pool.hpp>

#include <nuschl/unittests/helper.hpp>
#include <nuschl/unittests/vector_printer.hpp>

#include <sstream>

using namespace nuschl;
using namespace std::string_literals;
namespace bdata = boost::unit_test::data;

BOOST_AUTO_TEST_SUITE(BuiltinsPlus)

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Emtpy) {
    std::vector<s_exp_ptr> args;
    auto res = primitives::plus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}

BOOST_AUTO_TEST_CASE(Zero) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    auto res = primitives::plus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}

BOOST_AUTO_TEST_CASE(One) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    auto res = primitives::plus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(1, &pool));
}

BOOST_AUTO_TEST_CASE(List) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(2, &pool));
    args.push_back(make_number(3, &pool));
    auto res = primitives::plus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(6, &pool));
}

BOOST_AUTO_TEST_CASE(NotANumber) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(pool.create(make_atom("fo")));
    args.push_back(make_number(3, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::plus.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "+ expects only numbers as arguments."s == e.what();
        });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsMinus)
memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    auto res = primitives::minus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}
BOOST_AUTO_TEST_CASE(Minus) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    auto res = primitives::minus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}

BOOST_AUTO_TEST_CASE(One) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    auto res = primitives::minus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(-1, &pool));
}

BOOST_AUTO_TEST_CASE(SimpleMinus) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(1, &pool));
    auto res = primitives::minus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}

BOOST_AUTO_TEST_CASE(LongMinus) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(3, &pool));
    args.push_back(make_number(2, &pool));
    args.push_back(make_number(1, &pool));
    auto res = primitives::minus.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}

BOOST_AUTO_TEST_CASE(NotANumber) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(pool.create(make_atom("fo")));
    args.push_back(make_number(3, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::minus.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "- expects only numbers as arguments."s == e.what();
        });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsTimes)
memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    auto res = primitives::times.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(1, &pool));
}

BOOST_AUTO_TEST_CASE(Zero) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    auto res = primitives::times.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(0, &pool));
}

BOOST_AUTO_TEST_CASE(OneElement) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(4, &pool));
    auto res = primitives::times.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(4, &pool));
}

BOOST_AUTO_TEST_CASE(TwoElements) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(4, &pool));
    args.push_back(make_number(2, &pool));
    auto res = primitives::times.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(8, &pool));
}

BOOST_AUTO_TEST_CASE(List) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(4, &pool));
    args.push_back(make_number(3, &pool));
    args.push_back(make_number(2, &pool));
    auto res = primitives::times.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(24, &pool));
}

BOOST_AUTO_TEST_CASE(NotANumber) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(pool.create(make_atom("fo")));
    args.push_back(make_number(3, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::times.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "* expects only numbers as arguments."s == e.what();
        });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsDivide)
memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::divide.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "/ expects exactly 2 arguments, got 0."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(Zero) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::divide.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "/ expects exactly 2 arguments, got 1."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(TwoElements) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(4, &pool));
    args.push_back(make_number(2, &pool));
    auto res = primitives::divide.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(2, &pool));
}

BOOST_AUTO_TEST_CASE(List) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(12, &pool));
    args.push_back(make_number(3, &pool));
    args.push_back(make_number(2, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::divide.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "/ expects exactly 2 arguments, got 3."s == e.what();
        });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsEQ)
memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::eq.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "eq expects exactly 2 arguments, got 0."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(Zero) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::eq.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "eq expects exactly 2 arguments, got 1."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(List) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(12, &pool));
    args.push_back(make_number(3, &pool));
    args.push_back(make_number(2, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::eq.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "eq expects exactly 2 arguments, got 3."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(NumbersEqual) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(2, &pool));
    args.push_back(make_number(2, &pool));
    auto res = primitives::eq.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, s_exp::tru);
}

BOOST_AUTO_TEST_CASE(NumbersNotEqual) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(4, &pool));
    args.push_back(make_number(2, &pool));
    auto res = primitives::eq.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, s_exp::fals);
}

BOOST_AUTO_TEST_CASE(SymbolsEqual) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_atom("foo")));
    args.push_back(pool.create(make_atom("foo")));
    auto res = primitives::eq.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, s_exp::tru);
}

BOOST_AUTO_TEST_CASE(SymbolsNotEqual) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_atom("foo"s)));
    args.push_back(pool.create(make_atom("bar"s)));
    auto res = primitives::eq.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, s_exp::fals);
}

BOOST_AUTO_TEST_CASE(SymbolsNotEqualNumber) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_atom("1"s)));
    args.push_back(pool.create(make_atom(number{1})));
    auto res = primitives::eq.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, s_exp::fals);
}

std::vector<s_exp_ptr> val = {
    s_exp::nil,
    pool.create(s_exp::nil, s_exp::nil),
    s_exp::tru,
    s_exp::fals,
    make_number(1, &pool),
    pool.create(make_atom("a")),
    pool.create(pool.create(make_atom("a")), pool.create(make_atom("a"))),
    pool.create(pool.create(make_atom("a")), s_exp::nil)};

std::vector<s_exp_ptr> exp = {s_exp::tru,  s_exp::tru,  s_exp::fals,
                              s_exp::fals, s_exp::fals, s_exp::fals,
                              s_exp::fals, s_exp::fals};

BOOST_DATA_TEST_CASE(Nil, bdata::make(val) ^ bdata::make(exp), a, e) {
    std::vector<s_exp_ptr> args;
    args.push_back(a);
    args.push_back(s_exp::nil);
    auto res = primitives::eq.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, e);
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsLess)
memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::less.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "< expects exactly 2 arguments, got 0."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(Zero) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::less.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "< expects exactly 2 arguments, got 1."s == e.what();
        });
}

std::vector<s_exp_ptr> val1 = {make_number(1, &pool), make_number(1, &pool),
                               make_number(2, &pool)};

std::vector<s_exp_ptr> val2 = {make_number(2, &pool), make_number(1, &pool),
                               make_number(1, &pool)};

std::vector<s_exp_ptr> exp = {s_exp::tru, s_exp::fals, s_exp::fals};

BOOST_DATA_TEST_CASE(Less,
                     bdata::make(val1) ^ bdata::make(val2) ^ bdata::make(exp),
                     a, b, e) {
    std::vector<s_exp_ptr> args;
    args.push_back(a);
    args.push_back(b);
    auto res = primitives::less.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, e);
}

BOOST_AUTO_TEST_CASE(NotANumber) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_atom("a")));
    args.push_back(make_number(2, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::less.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "< expects only numbers as arguments."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(List) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(12, &pool));
    args.push_back(make_number(3, &pool));
    args.push_back(make_number(2, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::less.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "< expects exactly 2 arguments, got 3."s == e.what();
        });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsGreater)
memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::greater.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "> expects exactly 2 arguments, got 0."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(Zero) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(0, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::greater.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "> expects exactly 2 arguments, got 1."s == e.what();
        });
}

std::vector<s_exp_ptr> val1 = {make_number(1, &pool), make_number(1, &pool),
                               make_number(2, &pool)};

std::vector<s_exp_ptr> val2 = {make_number(2, &pool), make_number(1, &pool),
                               make_number(1, &pool)};

std::vector<s_exp_ptr> exp = {s_exp::fals, s_exp::fals, s_exp::tru};

BOOST_DATA_TEST_CASE(Less,
                     bdata::make(val1) ^ bdata::make(val2) ^ bdata::make(exp),
                     a, b, e) {
    std::vector<s_exp_ptr> args;
    args.push_back(a);
    args.push_back(b);
    auto res = primitives::greater.execute(args, &pool);
    BOOST_CHECK_EQUAL(res, e);
}

BOOST_AUTO_TEST_CASE(NotANumber) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_atom("a")));
    args.push_back(make_number(2, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::greater.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "> expects only numbers as arguments."s == e.what();
        });
}

BOOST_AUTO_TEST_CASE(List) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(12, &pool));
    args.push_back(make_number(3, &pool));
    args.push_back(make_number(2, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::greater.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "> expects exactly 2 arguments, got 3."s == e.what();
        });
}

BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsCar)

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(car) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_number(12, &pool), make_number(3, &pool)));
    auto res = primitives::car.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(12, &pool));
}

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::car.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "car expects exactly 1 arguments, got 0."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(Nil) {
    std::vector<s_exp_ptr> args;
    args.push_back(s_exp::nil);
    BOOST_CHECK_EXCEPTION(
        primitives::car.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "car expects a list as argument."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(Number) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::car.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "car expects a list as argument."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(ToManyArguments) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(1, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::car.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "car expects exactly 1 arguments, got 2."s == e.what();
        });
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsCdr)

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(cdr) {
    std::vector<s_exp_ptr> args;
    args.push_back(pool.create(make_number(12, &pool), make_number(3, &pool)));
    auto res = primitives::cdr.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *make_number(3, &pool));
}

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::cdr.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "cdr expects exactly 1 arguments, got 0."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(Nil) {
    std::vector<s_exp_ptr> args;
    args.push_back(s_exp::nil);
    BOOST_CHECK_EXCEPTION(
        primitives::cdr.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "cdr expects a list as argument."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(Number) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::cdr.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "cdr expects a list as argument."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(ToManyArguments) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(1, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::cdr.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "cdr expects exactly 1 arguments, got 2."s == e.what();
        });
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsCons)

memory::s_exp_pool pool;

BOOST_AUTO_TEST_CASE(cons) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(12, &pool));
    args.push_back(make_number(3, &pool));
    auto res = primitives::cons.execute(args, &pool);
    auto a = make_number(12, &pool);
    auto b = make_number(3, &pool);
    auto exp = pool.create(a, b);
    BOOST_CHECK_EQUAL(*res, *exp);
}

BOOST_AUTO_TEST_CASE(cons2) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(12, &pool));
    args.push_back(s_exp::nil);
    auto res = primitives::cons.execute(args, &pool);
    auto a = make_number(12, &pool);
    auto b = s_exp::nil;
    auto exp = pool.create(a, b);
    BOOST_CHECK_EQUAL(*res, *exp);
}

BOOST_AUTO_TEST_CASE(Empty) {
    std::vector<s_exp_ptr> args;
    BOOST_CHECK_EXCEPTION(
        primitives::cons.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "cons expects exactly 2 arguments, got 0."s == e.what();
        });
}
BOOST_AUTO_TEST_CASE(Nil) {
    std::vector<s_exp_ptr> args;
    args.push_back(s_exp::nil);
    args.push_back(s_exp::nil);
    auto res = primitives::cons.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *s_exp::nil);
}

BOOST_AUTO_TEST_CASE(ToManyArguments) {
    std::vector<s_exp_ptr> args;
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(1, &pool));
    args.push_back(make_number(1, &pool));
    BOOST_CHECK_EXCEPTION(
        primitives::cons.execute(args, &pool), nuschl::eval_argument_error,
        [](const nuschl::eval_argument_error &e) {
            return "cons expects exactly 2 arguments, got 3."s == e.what();
        });
}
BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE(BuiltinsList)

memory::s_exp_pool pool;

auto one = make_number(1, &pool);
auto two = make_number(2, &pool);
auto three = make_number(3, &pool);

std::vector<std::vector<s_exp_ptr>> val = {
    {}, {one}, {one, two}, {three, one, two}};

auto list1 = pool.create(one, s_exp::nil);
auto list2 = pool.create(one, pool.create(two, s_exp::nil));
auto list3 = pool.create(three, list2);

std::vector<s_exp_ptr> exp = {s_exp::nil, list1, list2, list3};

BOOST_DATA_TEST_CASE(List, bdata::make(val) ^ bdata::make(exp), args, e) {
    auto res = primitives::list.execute(args, &pool);
    BOOST_CHECK_EQUAL(*res, *e);
}

BOOST_AUTO_TEST_SUITE_END()
