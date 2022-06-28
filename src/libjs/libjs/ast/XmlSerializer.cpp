#include <libjs/ast/XmlSerializer.hpp>

#include <iostream>
#include <string>

namespace js::ast {

void XmlSerializer::exec(Program& program, std::ostream& out) {
  XmlSerializer xml_serializer;
  xml_serializer.nodes_.push(xml_serializer.doc_.append_child("program"));
  program.get_node()->accept(xml_serializer);
  xml_serializer.doc_.save(out, "  ");
}

void XmlSerializer::visit(Statement& value) {
  // auto key = append_child("statement");
  // nodes_.push(key);
  value.value()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Elements& value) {
  // auto key = append_child("elements");
  // nodes_.push(key);
  auto exprs = value.value();
  for (auto* expr : exprs) {
    expr->accept(*this);
  }
  nodes_.pop();
}

void XmlSerializer ::visit(Element& value) {
  // auto key = append_child("element");
  // nodes_.push(key);
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

  nodes_.pop();
}

void XmlSerializer ::visit(Value_types& value) {
  auto key = append_child("value");
  nodes_.push(key);
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

  // value.value()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Value_calc& value) {
  auto key = append_child("value for calc");
  nodes_.push(key);
  Node* r1 = value.val();
  auto* num = dynamic_cast<Number*>(r1);
  auto* ind = dynamic_cast<Name*>(r1);

  if (num != nullptr) {
    num->accept(*this);
  }
  if (ind != nullptr) {
    ind->accept(*this);
  }

  // value.value()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Declaration& value) {
  auto key = append_child("variable declaration");
  nodes_.push(key);
  key = append_child("name");
  nodes_.push(key);
  append_text(value.id().c_str());
  nodes_.pop();
  value.val()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Assigment& value) {
  auto key = append_child("assigment");
  nodes_.push(key);
  key = append_child("id");
  nodes_.push(key);
  append_text(value.id().c_str());
  nodes_.pop();
  value.val()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Number& value) {
  auto key = append_child("number");
  nodes_.push(key);
  append_text(value.value().c_str());
  nodes_.pop();
}

void XmlSerializer ::visit(Name& value) {
  auto key = append_child("name");
  nodes_.push(key);
  append_text(value.value().c_str());
  nodes_.pop();
}

void XmlSerializer ::visit(Calculations& value) {
  value.op1();
  auto key = append_child("calculations");
  nodes_.push(key);

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
    key = append_child("operation");

    nodes_.push(key);

    append_text(value.oper().c_str());
    nodes_.pop();

    auto* y = dynamic_cast<Calculations*>(value.op2());
    y->accept(*this);
  }
  nodes_.pop();
}

void XmlSerializer ::visit(If_elem& value) {
  auto key = append_child("if");
  nodes_.push(key);
  value.log()->accept(*this);
  value.exprs()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(While_elem& value) {
  auto key = append_child("while");
  nodes_.push(key);
  value.log()->accept(*this);
  value.exprs()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Compare& value) {
  auto key = append_child("compare");
  nodes_.push(key);
  value.op1()->accept(*this);
  key = append_child("op");
  nodes_.push(key);
  append_text(value.oper().c_str());
  nodes_.pop();
  value.op2()->accept(*this);
  nodes_.pop();
}

void XmlSerializer ::visit(Alert& value) {
  auto key = append_child("alert");
  nodes_.push(key);
  value.val()->accept(*this);
  nodes_.pop();
  value.val();
}

pugi::xml_node XmlSerializer::append_child(const char* name) {
  return nodes_.top().append_child(name);
}

void XmlSerializer::append_text(const char* text) {
  auto text_node = nodes_.top().append_child(pugi::node_pcdata);
  text_node.set_value(text);
}

}  // namespace js::ast
