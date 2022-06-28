set(lib_name js)

add_library(${lib_name})

include(CompileOptions)
js_target_set_compile_options(${lib_name})

target_include_directories(${lib_name} PUBLIC .)

target_sources(
  ${lib_name}
  PUBLIC
    libjs/ast/Ast.hpp
    libjs/ast/Visitor.hpp
    libjs/ast/XmlSerializer.hpp
    libjs/dump_tokens.hpp
    libjs/parser.hpp
    libjs/symtab/symtab.hpp
    libjs/semantics/sem.hpp
    libjs/codegen/codegen.hpp
    libjs/symbol_table.hpp
  PRIVATE
    libjs/ast/detail/Builder.cpp
    libjs/ast/detail/Builder.hpp
    libjs/ast/Ast.cpp
    libjs/ast/XmlSerializer.cpp
    libjs/dump_tokens.cpp
    libjs/parser.cpp
    libjs/symtab/symtab.cpp
    libjs/semantics/sem.cpp
    libjs/codegen/codegen.cpp
    libjs/symbol_table.cpp
)

target_link_libraries(
  ${lib_name}
  PRIVATE
    Js
    fmt
    pugixml
)

set(test_name libjs_test)

add_executable(${test_name})

add_test(NAME ${test_name} COMMAND ${test_name})

js_target_set_compile_options(${test_name})

target_sources(
  ${test_name}
  PRIVATE
    test/lexer.cpp
    test/parser.cpp
)

target_link_libraries(
  ${test_name}
  PRIVATE
    ${lib_name}
    gtest
    gtest_main
)
