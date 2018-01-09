find_package(Boost 1.59.0 REQUIRED COMPONENTS unit_test_framework)

set(NUSCHL_UNITTEST_LIB_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/helper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/string_to_s_exp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/s_exp_to_string.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/unique.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/vector_printer.cpp
  )

add_library(nuschltestlib ${NUSCHL_UNITTEST_LIB_SRC})
target_include_directories(nuschltestlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nuschltestlib PRIVATE nuschl-lib)


set(NUSCHL_UNITTESTS_SRC
${CMAKE_CURRENT_SOURCE_DIR}/unittests/main.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/atom.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/builtins.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/environment.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/exceptions.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/function.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/nil.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/number.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/primitives_impl.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/readfile.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/symbol.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/s_exp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/s_exp_helpers.cpp

${CMAKE_CURRENT_SOURCE_DIR}/unittests/input.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/parseerror.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/parser.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/token.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/token_iterator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/reverse_token_iterator.cpp

${CMAKE_CURRENT_SOURCE_DIR}/unittests/interpreter.cpp
)

add_executable(nuschltests ${NUSCHL_UNITTESTS_SRC} )
target_link_libraries(nuschltests PRIVATE nuschl-lib Boost::unit_test_framework nuschltestlib)

add_test(testnuschl nuschltests)
