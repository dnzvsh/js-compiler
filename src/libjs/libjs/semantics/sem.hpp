#pragma once

#include <iostream>
#include <libjs/ast/Ast.hpp>
#include <libjs/ast/Visitor.hpp>
#include <libjs/symbol_table.hpp>
#include <libjs/symtab/symtab.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace js::seman {

class semanVisitor : public ast::Visitor {
 public:
  void fillsymtab(js::ast::Program& prog);
  semanVisitor() {
    currentscope_ = new symtab::level;
    currentscope_->prev_ = nullptr;
  }
  void visit(js::ast::Statement& value) override;
  void visit(js::ast::Elements& value) override;
  void visit(js::ast::Element& value) override;
  void visit(js::ast::Value_types& value) override;
  void visit(js::ast::Value_calc& value) override;
  void visit(js::ast::Declaration& value) override;
  void visit(js::ast::Assigment& value) override;
  void visit(js::ast::Number& value) override;
  void visit(js::ast::Name& value) override;
  void visit(js::ast::Calculations& value) override;
  void visit(js::ast::If_elem& value) override;
  void visit(js::ast::While_elem& value) override;
  void visit(js::ast::Compare& value) override;

  symtab::level* get_scope() { return currentscope_; }
  std::vector<std::string> errors() { return errors_; }

 private:
  void scopeup() {}
  void scopedown() {}
  auto find(const std::string& id);

  std::vector<std::string> errors_;
  symtab::level* currentscope_;
};

}  // namespace js::seman
