#include <libjs/semantics/sem.hpp>

namespace js::seman {

using namespace ast;  // NOLINT

auto semanVisitor::find(const std::string& id) {
  auto* current = currentscope_;
  auto retvalfalse = current->levelsymbols_.end();
  do {
    auto r = current->levelsymbols_.find(id);
    if (r != current->levelsymbols_.end()) {
      return r;
    }

    current = current->prev_;
  } while (current != nullptr);
  return retvalfalse;
}

void semanVisitor::fillsymtab(Program& prog) {
  auto* start_rule = dynamic_cast<Statement*>(prog.get_node());
  start_rule->accept(*this);
}

void semanVisitor::visit(Statement& value) {
  auto* rl = value.value();
  rl->accept(*this);
}

void semanVisitor::visit(Elements& value) {
  auto exprs = value.value();
  for (auto* expr : exprs) {
    expr->accept(*this);
  }
}

void semanVisitor::visit(Element& value) {
  Node* rl = value.value();
  auto* decl = dynamic_cast<Declaration*>(rl);
  auto* assig = dynamic_cast<Assigment*>(rl);
  auto* cond = dynamic_cast<If_elem*>(rl);
  auto* lp = dynamic_cast<While_elem*>(rl);

  if (decl != nullptr) {
    decl->accept(*this);
  }
  if (assig != nullptr) {
    assig->accept(*this);
  }
  if (cond != nullptr) {
    cond->accept(*this);
  }
  if (lp != nullptr) {
    lp->accept(*this);
  }
}

void semanVisitor::visit(Value_types& value) {
  Node* r1 = value.val();
  auto* num = dynamic_cast<Number*>(r1);
  auto* ind = dynamic_cast<Name*>(r1);
  auto* calc = dynamic_cast<Calculations*>(r1);

  if (num != nullptr) {
    num->accept(*this);
  }
  if (ind != nullptr) {
    ind->accept(*this);
  }
  if (calc != nullptr) {
    calc->accept(*this);
  }
}

void semanVisitor::visit(Value_calc& value) {
  Node* r1 = value.val();
  auto* num = dynamic_cast<Number*>(r1);
  auto* ind = dynamic_cast<Name*>(r1);

  if (num != nullptr) {
    num->accept(*this);
  }
  if (ind != nullptr) {
    ind->accept(*this);
  }
}

void semanVisitor::visit(Declaration& value) {
  std::string id = value.id();
  currentscope_->levelsymbols_.find(id);

  value.val()->accept(*this);
}

void semanVisitor::visit(Assigment& value) {
  value.id();

  value.val()->accept(*this);
}

void semanVisitor::visit(Number& value) {
  value.value();
}

void semanVisitor::visit(Name& value) {
  find(value.value());
}

void semanVisitor::visit(Calculations& value) {
  value.op1();
  if (value.op2() == nullptr) {
    auto* val = value.op1();
    auto* val1 = dynamic_cast<Value_calc*>(val);
    auto* calc = dynamic_cast<Calculations*>(val);

    if (val1 != nullptr) {
      val1->accept(*this);
    }
    if (calc != nullptr) {
      calc->accept(*this);
    }
  } else {
    auto* x = dynamic_cast<Calculations*>(value.op1());
    x->accept(*this);
    auto* y = dynamic_cast<Calculations*>(value.op2());
    y->accept(*this);
  }
}

void semanVisitor::visit(If_elem& value) {
  value.log()->accept(*this);
  scopeup();
  value.exprs()->accept(*this);
  scopedown();
}

void semanVisitor::visit(While_elem& value) {
  value.log()->accept(*this);
  scopeup();
  value.exprs()->accept(*this);
  scopedown();
}

void semanVisitor::visit(Compare& value) {
  value.op1()->accept(*this);
  value.op2()->accept(*this);
}

symtabResult symbol_table(ast::Program& prog) {
  symtab::symtabVisitor st;
  st.fillsymtab(prog);
  if (!st.errors().empty()) {
    return symtabResult::errors(st.errors());
  }

  return symtabResult::symtab(st.get_scope());
}

}  // namespace js::seman
