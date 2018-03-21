################################
#
# Examples and Tests
#
################################

if local_boost
    clangboost_test = meson.get_compiler('cpp').find_library('boost_unit_test_framework', dirs: boost_lib_path, required: true)
    clangboost_log = meson.get_compiler('cpp').find_library('boost_log', dirs: boost_lib_path, required: true)
    clangboost_log_setup = meson.get_compiler('cpp').find_library('boost_log_setup', dirs: boost_lib_path, required: true)
    clangboost_timer = meson.get_compiler('cpp').find_library('boost_timer', dirs: boost_lib_path, required: true)
    boost_test_dep = declare_dependency(include_directories: clangboost_inc, dependencies: [clangboost_log_setup, clangboost_log, clangboost_test, clangboost_timer], link_args: '-L'+boost_lib_path)
else
    boost_test_dep = dependency('boost', modules: ['unit_test_framework'], required: true, version: '>=1.59.0')
endif

test_lib_files = files(
    'lib/helper.cpp',
    'lib/parsestring.cpp',
    'lib/string_to_s_exp.cpp',
    'lib/s_exp_to_string.cpp',
    'lib/unique.cpp',
    'lib/vector_printer.cpp'
)

nuschl_test_inc = include_directories('include')
nuschl_test_lib = static_library('nuschl-test', test_lib_files, include_directories: nuschl_test_inc, dependencies: [nuschl_dep, boost_test_dep])
nuschl_test_dep = declare_dependency(include_directories: nuschl_test_inc, link_with: nuschl_test_lib)

unit_test_files = [
    'unittests/main.cpp',
    'unittests/atom.cpp',
    'unittests/builtins.cpp',
    'unittests/environment.cpp',
    'unittests/exceptions.cpp',
    'unittests/function.cpp',
    'unittests/nil.cpp',
    'unittests/number.cpp',
    'unittests/primitives_impl.cpp',
    'unittests/readfile.cpp',
    'unittests/symbol.cpp',
    'unittests/s_exp.cpp',
    'unittests/s_exp_helpers.cpp',
    'unittests/s_exp_pool.cpp',

    'unittests/input.cpp',
    'unittests/parseerror.cpp',
    'unittests/parser.cpp',
    'unittests/token.cpp',
    'unittests/token_iterator.cpp',
    'unittests/reverse_token_iterator.cpp',

    'unittests/interpreter.cpp',
    'unittests/interpreter3.cpp'
]

exe = executable('unittests', unit_test_files, dependencies: [nuschl_dep, boost_test_dep, nuschl_test_dep])
test('unittests', exe)

#####################
# Compile time tests
#####################

exe = executable('is_printable', 'compiletime/is_printable.cpp', include_directories: nuschl_test_inc )
