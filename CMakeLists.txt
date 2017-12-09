cmake_minimum_required(VERSION 3.2)

project(nuschl CXX)

find_package(Boost 1.59.0 QUIET COMPONENTS unit_test_framework)

#set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "--std=c++17")

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

add_library(nuschl ${NUSCHL_LIB_FILES})
target_include_directories(nuschl PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if(Boost_FOUND)
	enable_testing()
	message("Tests enabled")
	add_subdirectory("test")
else()
	message("Tests disabled, boost unit testing framework required")
endif()
