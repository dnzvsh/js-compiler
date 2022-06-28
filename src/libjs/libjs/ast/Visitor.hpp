#pragma once

#include <libjs/ast/Ast.hpp>

namespace js::ast {

class Visitor {
 public:
  virtual void visit(Statement& node) = 0;
  virtual void visit(Elements& node) = 0;
  virtual void visit(Element& node) = 0;

  virtual void visit(Value_types& node) = 0;
  virtual void visit(Value_calc& node) = 0;

  virtual void visit(Declaration& node) = 0;
  virtual void visit(Assigment& node) = 0;

  virtual void visit(Name& node) = 0;
  virtual void visit(Number& node) = 0;

  virtual void visit(While_elem& node) = 0;
  virtual void visit(If_elem& node) = 0;

  virtual void visit(Calculations& node) = 0;
  virtual void visit(Compare& node) = 0;

  virtual void visit(Alert& node) = 0;
};

}  // namespace js::ast
