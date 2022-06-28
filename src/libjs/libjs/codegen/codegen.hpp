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

namespace js {

namespace codegen {

class codegenVisitor : public ast::Visitor {
 public:
  void CreateIR(ast::Program& doc);
  explicit codegenVisitor(symtab::level* scope) {
    currentscope_ = scope;
    number_ = 1;
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
  void visit(js::ast::Alert& value) override;

  std::stringstream& result() { return result_; }

 private:
  void scopeup() {
    currentscope_ = &(currentscope_->nested_[currentscope_->currentnested_]);
  }

  void scopedown() {
    currentscope_->currentnested_ = 0;
    currentscope_ = currentscope_->prev_;
    currentscope_->currentnested_++;
  }

  int jumpnumber() { return number_++; }
  std::string getreg() {
    std::stringstream s;
    s << "%tmp" << number_;
    number_++;
    return s.str();
  }

  auto find(const std::string& id);

  int number_;
  symtab::level* currentscope_;
  std::string reg_;
  std::stringstream result_;
};

}  // namespace codegen

}  // namespace js