find_package(Boost 1.59.0 REQUIRED COMPONENTS unit_test_framework)

set(NUSCHL_UNITTESTS_SRC
${CMAKE_CURRENT_SOURCE_DIR}/unittests/main.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/atom.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/environment.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/function.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/helper.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/helper.hpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/number.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/primitives.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/s_exp.cpp
${CMAKE_CURRENT_SOURCE_DIR}/unittests/symbol.cpp
)

add_executable(nuschltests ${NUSCHL_UNITTESTS_SRC} )
target_link_libraries(nuschltests PRIVATE nuschl)
target_link_libraries(nuschltests PRIVATE Boost::unit_test_framework)

add_test(testnuschl nuschltests)
