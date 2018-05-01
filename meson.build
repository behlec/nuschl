project('nuschl', 'cpp', default_options : ['cpp_std=c++1z'])

################################
#
# Configuration
#
################################

version = '0.1-dev'

# We compile with libc++
if get_option('libcpp')
    message('Using libcpp')
    add_project_arguments(['-stdlib=libc++'], language : 'cpp')
    add_project_link_arguments(['-stdlib=libc++'], language : 'cpp')
    if get_option('libcpp_link_path') != ''
        add_project_link_arguments([ '-I'+get_option('libcpp_inc_path'), '-L'+get_option('libcpp_link_path')], language : 'cpp')
    endif
endif

if meson.get_compiler('cpp').get_id() == 'clang' and not get_option('libcpp')
    message('Using boost::variant for clang')
    add_project_arguments(['-DNUSCHL_BOOSTVARIANT'], language : 'cpp')
    boost_variant = true
else
    message('Using std::variant')
    boost_variant = false
endif


###########
# Boost
###########

local_boost = false # do we use a user specified boost installation? Default: false

if get_option('boost_path') != '' # boost_path is set
    message('Base path to boost: ' + get_option('boost_path'))
    boost_inc_path = get_option('boost_path')+'/include'
    boost_lib_path = get_option('boost_path')+'/lib'
    local_boost = true
endif
if (((get_option('boost_inc_path') != '') and (get_option('boost_lib_path') == '')) or # xor(boost_inc_path set, boost_lib_path set)
    ((get_option('boost_inc_path') == '') and (get_option('boost_lib_path') != '')))
    error('Must set either both of boost_inc_path and boost_lib_path or none\nUse boost_path to set base to both')
endif
if ((get_option('boost_inc_path') != '') or (get_option('boost_lib_path') != ''))
    boost_inc_path = get_option('boost_inc_path')
    boost_lib_path = get_option('boost_lib_path')
    message('Path to boost ' + boost_inc_path + ' ' + boost_lib_path)
    local_boost = true
endif

if local_boost
  message('Using local boost: ' + boost_inc_path + ' ' + boost_lib_path)
  clangboost_inc = include_directories(boost_inc_path)
  boost_dep = declare_dependency(include_directories: clangboost_inc, link_args: '-L'+boost_lib_path)
        add_project_link_arguments(['-Wl,-rpath,'+boost_lib_path], language : 'cpp')
else
  boost_dep = dependency('boost', required: true, version: '>=1.59.0')
endif

################################
#
# Build
#
################################

##############
# Lib
##############

nuschl_files = files(
    'src/lib/atom.cpp',
    'src/lib/builtins.cpp',
    'src/lib/default_env.cpp',
    'src/lib/environment.cpp',
    'src/lib/exceptions.cpp',
    'src/lib/function.cpp',
    'src/lib/number.cpp',
    'src/lib/primitives.cpp',
    'src/lib/primitives_impl.cpp',
    'src/lib/readfile.cpp',
    'src/lib/symbol.cpp',
    'src/lib/s_exp.cpp',
    'src/lib/s_exp_helpers.cpp',
    'src/lib/s_exp_pool.cpp',

    'src/lib/lexer.cpp',
    'src/lib/parser.cpp',
    'src/lib/parseerror.cpp',
    'src/lib/reverse_token_iterator.cpp',
    'src/lib/token.cpp',
    'src/lib/token_iterator.cpp',

    'src/lib/interpreter.cpp',
    'src/lib/interpreter3.cpp'
)

nuschl_inc = include_directories('include')

nuschl_lib = library('nuschl', nuschl_files, include_directories: nuschl_inc, dependencies: boost_dep)
nuschl_dep = declare_dependency(include_directories: nuschl_inc, link_with: nuschl_lib)


##############
# Executables
##############

lexer = executable('lexer', 'src/bin/lexer.cpp', dependencies: nuschl_dep)
lexer = executable('parser', 'src/bin/parser.cpp', dependencies: nuschl_dep)
lexer = executable('nuschl', 'src/bin/nuschl.cpp', dependencies: nuschl_dep)
lexer = executable('nuschl3', 'src/bin/nuschl3.cpp', dependencies: nuschl_dep)

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

########
# Tests
########

subdir('test')


################################
#
# Doygen documentation
#
################################

# doxygen = find_program('doxygen')
# if doxygen.found()
#     run_target('doxy', command: ['doxygen', 'doc/Doxyfile'])
# else
#     message('Doygen not found')
# endif
