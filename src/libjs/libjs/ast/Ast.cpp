#include <libjs/ast/Ast.hpp>

#include <libjs/ast/Visitor.hpp>

#include <iostream>

namespace js::ast {

void Statement::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Elements::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Element::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Value_types::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Value_calc::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Declaration::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Assigment::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Number::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Name::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Calculations::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void If_elem::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void While_elem::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Compare::accept(Visitor& visitor) {
  visitor.visit(*this);
}

void Alert::accept(Visitor& visitor) {
  visitor.visit(*this);
}

}  // namespace js::ast
