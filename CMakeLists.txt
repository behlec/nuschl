cmake_minimum_required(VERSION 3.2)

project(nuschl CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NUSCHL_LIB_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/atom.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/builtins.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/default_env.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/environment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/exceptions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/function.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/number.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/primitives.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/primitives_impl.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/readfile.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/symbol.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/s_exp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/s_exp_pool.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/s_exp_helpers.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/lexer.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/parser.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/parseerror.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/reverse_token_iterator.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/token.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/token_iterator.cpp

  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/interpreter.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/interpreter3.cpp
)

find_package(Boost 1.59.0 COMPONENTS unit_test_framework)

add_subdirectory("test")
enable_testing()

add_library(nuschl-lib ${NUSCHL_LIB_FILES})

# clang need libc++ or boost
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (Boost_FOUND)
    message("Use boost variant")
    target_link_libraries(nuschl-lib Boost::boost)
    target_compile_definitions(nuschl-lib PUBLIC "-DNUSCHL_BOOSTVARIANT")
  else()
    message(FATAL_ERROR "Clang needs boost to compile")
  endif()
endif()

target_include_directories(nuschl-lib PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

add_executable(lexer ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/lexer.cpp)
target_link_libraries(lexer PRIVATE nuschl-lib)

add_executable(parser ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/parser.cpp)
target_link_libraries(parser PRIVATE nuschl-lib)

add_executable(nuschl ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/nuschl.cpp)
target_link_libraries(nuschl PRIVATE nuschl-lib)

add_executable(nuschl3 ${CMAKE_CURRENT_SOURCE_DIR}/src/bin/nuschl3.cpp)
target_link_libraries(nuschl3 PRIVATE nuschl-lib)
