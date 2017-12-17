cmake_minimum_required(VERSION 3.2)

project(nuschl CXX)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(NUSCHL_LIB_FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/atom.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/environment.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/exceptions.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/function.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/number.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/primitives.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/s_exp.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/src/lib/symbol.cpp
)

find_package(Boost 1.45.0 COMPONENTS unit_test_framework)

if(Boost_FOUND)
  if (Boost_UNIT_TEST_FRAMEWORK_FOUND)
    if (Boost_VERSION VERSION_GREATER "105900")
      message("Tests enabled")
      enable_testing()
      add_subdirectory("test")
    else()
      message("Tests disabled, need at least version 1.59.0")
    endif()
  else()
    message("Tests disabled, boost unit testing framework required")
  endif()
else()
  message("Boost not found")
endif()

add_library(nuschl ${NUSCHL_LIB_FILES})

# clang need libc++ or boost
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang")
  if (Boost_FOUND)
    message("Use boost variant")
    target_link_libraries(nuschl Boost::boost)
    target_compile_definitions(nuschl PUBLIC "-DNUSCHL_BOOSTVARIANT")
  else()
    message(FATAL_ERROR "Clang needs boost to compile")
  endif()
endif()

target_include_directories(nuschl PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)
