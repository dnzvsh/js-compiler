#pragma once

#include <iostream>
#include <libjs/ast/Ast.hpp>
#include <libjs/ast/Visitor.hpp>
#include <sstream>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace js::symtab {

enum class idtype { VAR = 0, FUNC, ARR };

struct symbol {
  std::string idtype_;

  std::string reg_;
};

struct level {  // NOLINT
  level* prev_;
  std::vector<level> nested_;
  std::unordered_map<std::string, symbol> levelsymbols_;
  int currentnested_ = 0;
};

class symtabVisitor : public js::ast::Visitor {
 public:
  void fillsymtab(js::ast::Program& program);
  symtabVisitor() {
    currentscope_ = new level;
    currentscope_->prev_ = nullptr;
  }

  void visit(ast::Statement& value) override;
  void visit(ast::Elements& value) override;
  void visit(ast::Element& value) override;
  void visit(ast::Value_types& value) override;
  void visit(ast::Value_calc& value) override;
  void visit(ast::Declaration& value) override;
  void visit(ast::Assigment& value) override;
  void visit(ast::Number& value) override;
  void visit(ast::Name& value) override;
  void visit(ast::Calculations& value) override;
  void visit(ast::If_elem& value) override;
  void visit(ast::While_elem& value) override;
  void visit(ast::Compare& value) override;
  void visit(ast::Alert& value) override;

  level* get_scope() { return currentscope_; }
  std::vector<std::string> errors() { return errors_; }

 private:
  void scopeup() {
    level m;
    m.prev_ = currentscope_;
    m.currentnested_ = 0;
    currentscope_->nested_.push_back(m);

    currentscope_ = &(currentscope_->nested_.back());
  }

  void scopedown() { currentscope_ = currentscope_->prev_; }
  auto find(const std::string& id);

  std::vector<std::string> errors_;
  level* currentscope_;
  std::string ftype_;
};

}  // namespace js::symtab