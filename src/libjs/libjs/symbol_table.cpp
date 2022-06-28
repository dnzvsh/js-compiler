#include <libjs/symbol_table.hpp>

namespace js {

symtabResult symbol_table(ast::Program& program) {
  symtab::symtabVisitor st;
  st.fillsymtab(program);

  if (!st.errors().empty()) {
    return symtabResult::errors(st.errors());
  }

  return symtabResult::symtab(st.get_scope());
}

void dump_errors_st(const std::vector<std::string>& errors, std::ostream& out) {
  for (const auto& error : errors) {
    out << error;
  }
}

}  // namespace js