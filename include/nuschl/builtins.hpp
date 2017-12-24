#pragma once

#include <nuschl/primitives.hpp>
#include <nuschl/s_exp_helpers.hpp>
#include <nuschl/util/primitives_impl.hpp>

#include <cassert>
#include <numeric>

namespace nuschl::primitive_impl {

primitivebuilder plus("+",
                      [](const std::vector<s_exp_ptr> &arguments,
                         memory::s_exp_pool *pool) -> const s_exp * {
                          assert(pool != nullptr);
                          number sum{0};
                          std::vector<number> tmp;
                          to_numbers(arguments.begin(), arguments.end(),
                                     std::back_inserter(tmp));
                          sum = std::accumulate(tmp.begin(), tmp.end(), sum);
                          return pool->create(make_atom(number{sum}));
                      },
                      all_numbers());

primitivebuilder minus("-",
                       [](const std::vector<s_exp_ptr> &arguments,
                          memory::s_exp_pool *pool) -> const s_exp * {
                           assert(pool != nullptr);
                           std::vector<number> tmp;
                           to_numbers(arguments.begin(), arguments.end(),
                                      std::back_inserter(tmp));
                           if (tmp.size() == 1) {
                               number res(-tmp[0]);
                               return pool->create(make_atom(number{res}));
                           } else {
                               number res(tmp[0]);
                               number sum{0};
                               sum = std::accumulate(tmp.begin() + 1, tmp.end(),
                                                     sum);
                               res = res - sum;
                               return pool->create(make_atom(number{res}));
                           }
                       },
                       all_numbers() && least_n_args(1));

primitivebuilder times(
    "*",
    [](const std::vector<s_exp_ptr> &arguments,
       memory::s_exp_pool *pool) -> const s_exp * {
        assert(pool != nullptr);
        number res{1};
        std::vector<number> tmp;
        to_numbers(arguments.begin(), arguments.end(), std::back_inserter(tmp));
        res = std::accumulate(tmp.begin(), tmp.end(), res,
                              [](const number &a, number &b) { return a * b; });
        return pool->create(make_atom(number{res}));
    },
    all_numbers());

primitivebuilder divide("/",
                        [](const std::vector<s_exp_ptr> &arguments,
                           memory::s_exp_pool *pool) -> const s_exp * {
                            assert(pool != nullptr);
                            std::vector<number> tmp;
                            to_numbers(arguments.begin(), arguments.end(),
                                       std::back_inserter(tmp));
                            number res(tmp[0]);
                            res = res / tmp[1];
                            return pool->create(make_atom(number{res}));
                        },
                        all_numbers() && exact_n_args(2));

primitivebuilder eq("eq",
                    [](const std::vector<s_exp_ptr> &arguments,
                       memory::s_exp_pool *pool) -> const s_exp * {
                        const s_exp *res = s_exp::fals;
                        if (*arguments[0] == *arguments[1]) {
                            res = s_exp::tru;
                        }
                        return res;
                    },
                    exact_n_args(2));

primitivebuilder less("<",
                      [](const std::vector<s_exp_ptr> &arguments,
                         memory::s_exp_pool *pool) -> const s_exp * {
                          assert(pool != nullptr);
                          std::vector<number> tmp;
                          to_numbers(arguments.begin(), arguments.end(),
                                     std::back_inserter(tmp));
                          if (tmp[0] < tmp[1]) {
                              return s_exp::tru;
                          } else {
                              return s_exp::fals;
                          }
                      },
                      all_numbers() && exact_n_args(2));

primitivebuilder greater(">",
                         [](const std::vector<s_exp_ptr> &arguments,
                            memory::s_exp_pool *pool) -> const s_exp * {
                             assert(pool != nullptr);
                             std::vector<number> tmp;
                             to_numbers(arguments.begin(), arguments.end(),
                                        std::back_inserter(tmp));
                             if (tmp[0] > tmp[1]) {
                                 return s_exp::tru;
                             } else {
                                 return s_exp::fals;
                             }
                         },
                         all_numbers() && exact_n_args(2));

primitivebuilder car("car",
                     [](const std::vector<s_exp_ptr> &arguments,
                        memory::s_exp_pool *pool) -> const s_exp * {
                         assert(pool != nullptr);
                         return arguments[0]->car();
                     },
                     is_list());

primitivebuilder cdr("cdr",
                     [](const std::vector<s_exp_ptr> &arguments,
                        memory::s_exp_pool *pool) -> const s_exp * {
                         assert(pool != nullptr);
                         return arguments[0]->cdr();
                     },
                     is_list());

primitivebuilder cons("cons",
                      [](const std::vector<s_exp_ptr> &arguments,
                         memory::s_exp_pool *pool) -> const s_exp * {
                          assert(pool != nullptr);
                          return pool->create(arguments[0], arguments[1]);
                      },
                      exact_n_args(2));

struct list : primitive {
    s_exp_ptr execute(const std::vector<s_exp_ptr> &arguments,
                      memory::s_exp_pool *pool) const override;
    std::string representation() const override;
};
}
