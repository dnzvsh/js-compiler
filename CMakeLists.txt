cmake_minimum_required(VERSION 3.21)

project(
  JSParser
  VERSION 1.0
  LANGUAGES CXX
)

list(
  APPEND
  CMAKE_MODULE_PATH
  ${CMAKE_CURRENT_LIST_DIR}/cmake
  ${CMAKE_CURRENT_LIST_DIR}/cmake/antlr4
)

find_program(
  CLANG_TIDY_EXE
  NAMES clang-tidy-10 clang-tidy
)

if(NOT CLANG_TIDY_EXE)
  message(WARNING "clang-tidy not found")
else()
  execute_process(
    COMMAND ${CLANG_TIDY_EXE} --version
    OUTPUT_VARIABLE clang_tidy_version
  )
  message("clang-tidy version:\n${clang_tidy_version}")
endif()

enable_testing()

add_subdirectory(src)
add_subdirectory(thirdparty)
