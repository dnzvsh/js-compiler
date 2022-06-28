#include <libjs/symtab/symtab.hpp>

namespace js::symtab {

using namespace ast;  // NOLINT

auto symtabVisitor::find(const std::string& id) {
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

void symtabVisitor::fillsymtab(Program& program) {
  auto* start_rule = dynamic_cast<Statement*>(program.get_node());
  start_rule->accept(*this);
}

void symtabVisitor::visit(Statement& value) {
  auto* rl = value.value();
  rl->accept(*this);
}

void symtabVisitor::visit(Elements& value) {
  auto exprs = value.value();
  for (auto* expr : exprs) {
    expr->accept(*this);
  }
}

void symtabVisitor::visit(Element& value) {
  Node* rl = value.value();
  auto* decl = dynamic_cast<Declaration*>(rl);
  auto* assig = dynamic_cast<Assigment*>(rl);
  auto* cond = dynamic_cast<If_elem*>(rl);
  auto* lp = dynamic_cast<While_elem*>(rl);
  auto* pr = dynamic_cast<Alert*>(rl);

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
  if (pr != nullptr) {
    pr->accept(*this);
  }
}

void symtabVisitor::visit(Value_types& value) {
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

void symtabVisitor::visit(Value_calc& value) {
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

void symtabVisitor::visit(Declaration& value) {
  std::string id = value.id();
  auto fres = currentscope_->levelsymbols_.find(id);
  if (fres == currentscope_->levelsymbols_.end()) {
    symbol sym;
    sym.idtype_ = "int";
    currentscope_->levelsymbols_.insert(std::pair(id, sym));
  } else {
    std::cout << id << '\n';
    std::stringstream s;
    s << "Variable redefinition (ERR): " << id << ".\n";

    errors_.push_back(s.str());
  }
  value.val()->accept(*this);
}

void symtabVisitor::visit(Assigment& value) {
  value.id();

  value.val()->accept(*this);
}

void symtabVisitor::visit(Number& value) {
  value.value();
}

void symtabVisitor::visit(Name& value) {
  auto fres = find(value.value());
  if (fres == currentscope_->levelsymbols_.end()) {
    std::stringstream s;
    s << "Undefined variable (ERR): " << value.value() << ".\n";
    errors_.push_back(s.str());
  }
}

void symtabVisitor::visit(Calculations& value) {
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

void symtabVisitor::visit(If_elem& value) {
  value.log()->accept(*this);
  scopeup();
  value.exprs()->accept(*this);
  scopedown();
}

void symtabVisitor::visit(While_elem& value) {
  value.log()->accept(*this);
  scopeup();
  value.exprs()->accept(*this);
  scopedown();
}

void symtabVisitor::visit(Compare& value) {
  value.op1()->accept(*this);
  value.op2()->accept(*this);
}

void symtabVisitor::visit(Alert& value) {
  value.val()->accept(*this);
}

}  // namespace js::symtab
