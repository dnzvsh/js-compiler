include(ExternalAntlr4Cpp)

set(lib_name antlr4)

add_library(${lib_name} INTERFACE)

target_link_libraries(
  ${lib_name}
  INTERFACE
    antlr4_static
)

target_include_directories(
  ${lib_name}
  SYSTEM
  INTERFACE
    ${ANTLR4_INCLUDE_DIRS}
)
