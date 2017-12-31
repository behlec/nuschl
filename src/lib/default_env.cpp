#include <nuschl/builtins.hpp>
#include <nuschl/default_env.hpp>

#include <string>

using namespace std::literals::string_literals;

namespace nuschl {
namespace defenv {
nuschl::s_exp plus(&primitives::plus);
nuschl::s_exp minus(&primitives::minus);
nuschl::s_exp less(&primitives::less);
nuschl::s_exp greater(&primitives::greater);
nuschl::s_exp times(&primitives::times);
nuschl::s_exp divide(&primitives::divide);
nuschl::s_exp eq(&primitives::eq);
nuschl::s_exp car(&primitives::car);
nuschl::s_exp cdr(&primitives::cdr);
nuschl::s_exp cons(&primitives::cons);
nuschl::s_exp list(&primitives::list);
}
}

nuschl::environment::table default_table() {
    nuschl::environment::table t;
    t.insert({"nil"s, nuschl::s_exp::nil});
    t.insert({"#f"s, nuschl::s_exp::fals});
    t.insert({"#t"s, nuschl::s_exp::tru});
    t.insert({"+"s, &nuschl::defenv::plus});
    t.insert({"-"s, &nuschl::defenv::minus});
    t.insert({"<"s, &nuschl::defenv::less});
    t.insert({">"s, &nuschl::defenv::greater});
    t.insert({"*"s, &nuschl::defenv::times});
    t.insert({"/"s, &nuschl::defenv::divide});
    t.insert({"eq"s, &nuschl::defenv::eq});
    t.insert({"car"s, &nuschl::defenv::car});
    t.insert({"cdr"s, &nuschl::defenv::cdr});
    t.insert({"cons"s, &nuschl::defenv::cons});
    t.insert({"list"s, &nuschl::defenv::list});
    return t;
}

const nuschl::environment
    nuschl::default_env(default_table(),
                        std::make_shared<nuschl::environment>());
