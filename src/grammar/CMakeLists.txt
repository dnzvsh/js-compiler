set(parser_lib_name Js)

add_library(${parser_lib_name} STATIC)

find_package(ANTLR REQUIRED)

set(antlr_parser_target_name Antlr${parser_lib_name})
antlr_target(
  ${antlr_parser_target_name}
  ${parser_lib_name}.g4
  LEXER
  PARSER
  VISITOR
)

include(CompileOptions)
target_set_cxx_stanrard(${parser_lib_name})

target_sources(
  ${parser_lib_name}
  PRIVATE
    ${ANTLR_${antlr_parser_target_name}_CXX_OUTPUTS}
)

target_include_directories(
  ${parser_lib_name}
  PUBLIC
    ${ANTLR_${antlr_parser_target_name}_OUTPUT_DIR}
)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)

target_link_libraries(
  ${parser_lib_name}
  PUBLIC
    antlr4
    Threads::Threads
)
