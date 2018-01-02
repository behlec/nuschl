find_package(Boost 1.59.0 REQUIRED COMPONENTS unit_test_framework)

set(NUSCHL_UNITTEST_LIB_SRC
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/helper.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/parse_res.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/print_test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/lib/vector_printer.cpp
  )

add_library(nuschltestlib ${NUSCHL_UNITTEST_LIB_SRC})
target_include_directories(nuschltestlib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_link_libraries(nuschltestlib PRIVATE nuschl)


set(NUSCHL_UNITTESTS_SRC
${CMAKE_CURRENT_SOURCE_DIR}/unittests/main.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/atom.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/builtins.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/environment.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/function.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/helper.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/nil.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/number.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/primitives_impl.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/readfile.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/symbol.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/s_exp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/s_exp_helpers.cpp

${CMAKE_CURRENT_SOURCE_DIR}/unittests/parser.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/token.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/token_iterator.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/reverse_token_iterator.cpp
)

add_executable(nuschltests ${NUSCHL_UNITTESTS_SRC} )
target_link_libraries(nuschltests PRIVATE nuschl)
target_link_libraries(nuschltests PUBLIC nuschltestlib)
target_link_libraries(nuschltests PRIVATE Boost::unit_test_framework)

add_test(testnuschl nuschltests)
