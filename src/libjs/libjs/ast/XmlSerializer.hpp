#pragma once

#include <libjs/ast/Visitor.hpp>

#include <pugixml.hpp>

#include <ostream>
#include <stack>

namespace js::ast {

class XmlSerializer final : public Visitor {
 public:
  static void exec(Program& program, std::ostream& out);

  void visit(Statement& value) override;
  void visit(Element& value) override;
  void visit(Elements& value) override;
  void visit(Value_types& value) override;
  void visit(Value_calc& value) override;
  void visit(Declaration& value) override;
  void visit(Assigment& value) override;
  void visit(Number& value) override;
  void visit(Name& value) override;
  void visit(Calculations& value) override;
  void visit(If_elem& value) override;
  void visit(While_elem& value) override;
  void visit(Compare& value) override;
  void visit(Alert& value) override;

 private:
  pugi::xml_node append_child(const char* name);
  void append_text(const char* text);

  pugi::xml_document doc_;
  std::stack<pugi::xml_node> nodes_;
};

}  // namespace js::ast
