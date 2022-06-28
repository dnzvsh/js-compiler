#pragma once

#include <iostream>
#include <libjs/symtab/symtab.hpp>
#include <sstream>
#include <string>

namespace js {

struct symtabResult {  // NOLINT
  static symtabResult errors(std::vector<std::string> errors) {
    symtabResult result;
    result.errors_ = errors;  // NOLINT
    return result;
  }

  static symtabResult symtab(symtab::level* globalscope) {
    symtabResult result;
    result.globalscope_ = globalscope;
    return result;
  }

  std::vector<std::string> errors_;
  symtab::level* globalscope_;
};

void dump_errors_st(const std::vector<std::string>& errors, std::ostream& out);

symtabResult symbol_table(ast::Program& program);

}  // namespace js