#pragma once

#include <nuschl/primitives.hpp>
#include <nuschl/util/primitives_impl.hpp>
#include <nuschl/util/s_exp_helpers.hpp>

#include <cassert>
#include <numeric>

namespace nuschl::primitives {

/**
 * \brief Add numbers.
 *
 * \param arguments Must be all numbers.
 */
primitive_impl::primitivebuilder
    plus("+",
         [](const std::vector<s_exp_ptr> &arguments,
            memory::s_exp_pool *pool) -> const s_exp * {
             assert(pool != nullptr);
             number sum{0};
             std::vector<number> tmp;
             primitive_impl::to_numbers(arguments.begin(), arguments.end(),
                                        std::back_inserter(tmp));
             sum = std::accumulate(tmp.begin(), tmp.end(), sum);
             return pool->create(make_atom(number{sum}));
         },
         primitive_impl::all_numbers());

/**
 * \brief Negate number or subtract numbers.
 *
 * If there is only one arguments, the argument ist negated.
 * Else takes the first argument and subtracts the remaining arguments.
 *
 * \param arguments There must be at least one argument and they must be all
 * numbers.
 */
primitive_impl::primitivebuilder
    minus("-",
          [](const std::vector<s_exp_ptr> &arguments,
             memory::s_exp_pool *pool) -> const s_exp * {
              assert(pool != nullptr);
              std::vector<number> tmp;
              primitive_impl::to_numbers(arguments.begin(), arguments.end(),
                                         std::back_inserter(tmp));
              if (tmp.size() == 1) {
                  number res(-tmp[0]);
                  return pool->create(make_atom(number{res}));
              } else {
                  number res(tmp[0]);
                  number sum{0};
                  sum = std::accumulate(tmp.begin() + 1, tmp.end(), sum);
                  res = res - sum;
                  return pool->create(make_atom(number{res}));
              }
          },
          primitive_impl::all_numbers() && primitive_impl::least_n_args(1));

/**
 * \brief Multiply numbers.
 *
 * \param arguments Must be all numbers.
 */
primitive_impl::primitivebuilder times(
    "*",
    [](const std::vector<s_exp_ptr> &arguments,
       memory::s_exp_pool *pool) -> const s_exp * {
        assert(pool != nullptr);
        number res{1};
        std::vector<number> tmp;
        primitive_impl::to_numbers(arguments.begin(), arguments.end(),
                                   std::back_inserter(tmp));
        res = std::accumulate(tmp.begin(), tmp.end(), res,
                              [](const number &a, number &b) { return a * b; });
        return pool->create(make_atom(number{res}));
    },
    primitive_impl::all_numbers());

/**
 * \brief Divide two numbers.
 *
 * \param arguments Number of arguments must be exactly two and numbers.
 */
primitive_impl::primitivebuilder
    divide("/",
           [](const std::vector<s_exp_ptr> &arguments,
              memory::s_exp_pool *pool) -> const s_exp * {
               assert(pool != nullptr);
               std::vector<number> tmp;
               primitive_impl::to_numbers(arguments.begin(), arguments.end(),
                                          std::back_inserter(tmp));
               number res(tmp[0]);
               res = res / tmp[1];
               return pool->create(make_atom(number{res}));
           },
           primitive_impl::all_numbers() && primitive_impl::exact_n_args(2));

/**
 * \brief Compares two elements.
 *
 * Test if two elements are equal.
 *
 * \param arguments Number of arguments must be exactly two.
 */
primitive_impl::primitivebuilder
    eq("eq",
       [](const std::vector<s_exp_ptr> &arguments,
          memory::s_exp_pool *pool) -> const s_exp * {
           const s_exp *a = arguments[0];
           const s_exp *b = arguments[1];
           const s_exp *res = s_exp::fals;
           // Handle nil and empty list
           if ((is_nil(a) || is_empty_cell(a)) &&
               (is_nil(b) || is_empty_cell(b))) {
               res = s_exp::tru;
           }
           // General case
           if (*a == *b) {
               res = s_exp::tru;
           }
           return res;
       },
       primitive_impl::exact_n_args(2));

/**
 * \brief Compares two elements for less.
 *
 * Test if the first element is less than the second.
 *
 * \param arguments Number of arguments must be exactly two and both must be
 * numbers.
 */
primitive_impl::primitivebuilder
    less("<",
         [](const std::vector<s_exp_ptr> &arguments,
            memory::s_exp_pool *pool) -> const s_exp * {
             assert(pool != nullptr);
             std::vector<number> tmp;
             primitive_impl::to_numbers(arguments.begin(), arguments.end(),
                                        std::back_inserter(tmp));
             if (tmp[0] < tmp[1]) {
                 return s_exp::tru;
             } else {
                 return s_exp::fals;
             }
         },
         primitive_impl::all_numbers() && primitive_impl::exact_n_args(2));

/**
 * \brief Compares two elements for greater.
 *
 * Test if the first element is greater than the second.
 *
 * \param arguments Number of arguments must be exactly two and both must be
 * numbers.
 */
primitive_impl::primitivebuilder
    greater(">",
            [](const std::vector<s_exp_ptr> &arguments,
               memory::s_exp_pool *pool) -> const s_exp * {
                assert(pool != nullptr);
                std::vector<number> tmp;
                primitive_impl::to_numbers(arguments.begin(), arguments.end(),
                                           std::back_inserter(tmp));
                if (tmp[0] > tmp[1]) {
                    return s_exp::tru;
                } else {
                    return s_exp::fals;
                }
            },
            primitive_impl::all_numbers() && primitive_impl::exact_n_args(2));

/**
 * \brief Get the first element of a cell.
 *
 * \param arguments Must be one element of type cell.
 */
primitive_impl::primitivebuilder
    car("car",
        [](const std::vector<s_exp_ptr> &arguments,
           memory::s_exp_pool *pool) -> const s_exp * {
            assert(pool != nullptr);
            return arguments[0]->car();
        },
        primitive_impl::is_list());

/**
 * \brief Get the second element of a cell.
 *
 * \param arguments Must be one element of type cell.
 */
primitive_impl::primitivebuilder
    cdr("cdr",
        [](const std::vector<s_exp_ptr> &arguments,
           memory::s_exp_pool *pool) -> const s_exp * {
            assert(pool != nullptr);
            return arguments[0]->cdr();
        },
        primitive_impl::is_list());

/**
 * \brief Create a cell from two elements.
 *
 * \param arguments Must be exactly two elements.
 */
primitive_impl::primitivebuilder
    cons("cons",
         [](const std::vector<s_exp_ptr> &arguments,
            memory::s_exp_pool *pool) -> const s_exp * {
             assert(pool != nullptr);
             return pool->create(arguments[0], arguments[1]);
         },
         primitive_impl::exact_n_args(2));

/**
 * \brief Create a list from elements.
 *
 * \param arguments No restrictions.
 */
struct list_impl : primitive {
    s_exp_ptr execute(const std::vector<s_exp_ptr> &arguments,
                      memory::s_exp_pool *pool) const override;
    std::string representation() const override;
};

extern list_impl list;
}
