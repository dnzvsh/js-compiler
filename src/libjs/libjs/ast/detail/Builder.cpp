#include <libjs/ast/detail/Builder.hpp>

namespace js::ast::detail {

std::any Builder::visitProgram(JsParser::ProgramContext* context) {
  auto* node = std::any_cast<Node*>(visitChildren(context));
  program_.set_node(node);
  return node;
}

std::any Builder::visitStatement(JsParser::StatementContext* context) {
  auto* value =
      dynamic_cast<Elements*>(std::any_cast<Node*>(visit(context->elements())));
  return static_cast<Node*>(program_.create_node<Statement>(value));
}

std::any Builder::visitElements(JsParser::ElementsContext* context) {
  std::vector<Element*> exprs;
  for (auto* el : context->element()) {
    exprs.push_back(dynamic_cast<Element*>(std::any_cast<Node*>(visit(el))));
  }
  return static_cast<Node*>(program_.create_node<Elements>(exprs));
}

std::any Builder::visitElement(JsParser::ElementContext* context) {
  if (context->declaration() != nullptr) {
    auto* decl = dynamic_cast<Declaration*>(
        std::any_cast<Node*>(visit(context->declaration())));
    return static_cast<Node*>(program_.create_node<Element>(decl));
  }
  if (context->assigment() != nullptr) {
    auto* assig = dynamic_cast<Assigment*>(
        std::any_cast<Node*>(visit(context->assigment())));
    return static_cast<Node*>(program_.create_node<Element>(assig));
  }
  if (context->if_elem() != nullptr) {
    auto* cond =
        dynamic_cast<If_elem*>(std::any_cast<Node*>(visit(context->if_elem())));
    return static_cast<Node*>(program_.create_node<Element>(cond));
  }
  if (context->while_elem() != nullptr) {
    auto* lop = dynamic_cast<While_elem*>(
        std::any_cast<Node*>(visit(context->while_elem())));
    return static_cast<Node*>(program_.create_node<Element>(lop));
  }
  if (context->alert() != nullptr) {
    auto* pr =
        dynamic_cast<Alert*>(std::any_cast<Node*>(visit(context->alert())));
    return static_cast<Node*>(program_.create_node<Element>(pr));
  }
  return 0;
}

std::any Builder::visitDeclaration(JsParser::DeclarationContext* context) {
  std::string id = context->name()->getText();
  auto* val = dynamic_cast<Value_types*>(
      std::any_cast<Node*>(visit(context->assignvalue()->value_types())));
  return static_cast<Node*>(program_.create_node<Declaration>(id, val));
}

std::any Builder::visitAssigment(JsParser::AssigmentContext* context) {
  std::string id = context->name()->getText();
  auto* val = dynamic_cast<Value_types*>(
      std::any_cast<Node*>(visit(context->assignvalue()->value_types())));
  return static_cast<Node*>(program_.create_node<Assigment>(id, val));
}

std::any Builder::visitWhile_elem(JsParser::While_elemContext* context) {
  auto* logres =
      dynamic_cast<Compare*>(std::any_cast<Node*>(visit(context->compare())));
  auto* exprsres =
      dynamic_cast<Elements*>(std::any_cast<Node*>(visit(context->elements())));

  return static_cast<Node*>(program_.create_node<While_elem>(logres, exprsres));
}

std::any Builder::visitIf_elem(JsParser::If_elemContext* context) {
  auto* logres =
      dynamic_cast<Compare*>(std::any_cast<Node*>(visit(context->compare())));
  auto* exprsres =
      dynamic_cast<Elements*>(std::any_cast<Node*>(visit(context->elements())));

  return static_cast<Node*>(program_.create_node<If_elem>(logres, exprsres));
}

std::any Builder::visitCalculations(JsParser::CalculationsContext* context) {
  if (context->value_calc() != nullptr) {
    auto* val1 = dynamic_cast<Value_calc*>(
        std::any_cast<Node*>(visit(context->value_calc())));
    return static_cast<Node*>(
        program_.create_node<Calculations>(val1, nullptr, std::string()));
  }
  if (context->calculations(1) != nullptr) {
    auto* calc1 = dynamic_cast<Calculations*>(
        std::any_cast<Node*>(visit(context->calculations(0))));
    auto* calc2 = dynamic_cast<Calculations*>(
        std::any_cast<Node*>(visit(context->calculations(1))));
    std::string calcop;
    if (context->lvl1op() != nullptr) {
      calcop = context->lvl1op()->getText();
    }
    if (context->lvl2op() != nullptr) {
      calcop = context->lvl2op()->getText();
    }
    return static_cast<Node*>(
        program_.create_node<Calculations>(calc1, calc2, calcop));
  }
  auto* calc1 = dynamic_cast<Calculations*>(
      std::any_cast<Node*>(visit(context->calculations(0))));
  return static_cast<Node*>(
      program_.create_node<Calculations>(calc1, nullptr, std::string()));
}

std::any Builder::visitValue_types(JsParser::Value_typesContext* context) {
  if (context->number() != nullptr) {
    auto* num =
        dynamic_cast<Number*>(std::any_cast<Node*>(visit(context->number())));
    return static_cast<Node*>(program_.create_node<Value_types>(num));
  }
  if (context->name() != nullptr) {
    auto* id =
        dynamic_cast<Name*>(std::any_cast<Node*>(visit(context->name())));
    return static_cast<Node*>(program_.create_node<Value_types>(id));
  }
  if (context->calculations() != nullptr) {
    auto* calc = dynamic_cast<Calculations*>(
        std::any_cast<Node*>(visit(context->calculations())));
    return static_cast<Node*>(program_.create_node<Value_types>(calc));
  }

  return static_cast<Node*>(program_.create_node<Value_types>(nullptr));
}

std::any Builder::visitNumber(JsParser::NumberContext* context) {
  std::string num = context->getText();
  return static_cast<Node*>(program_.create_node<Number>(num));
}

std::any Builder::visitName(JsParser::NameContext* context) {
  std::string id = context->getText();
  return static_cast<Node*>(program_.create_node<Name>(id));
}

std::any Builder::visitValue_calc(JsParser::Value_calcContext* context) {
  if (context->number() != nullptr) {
    auto* num =
        dynamic_cast<Number*>(std::any_cast<Node*>(visit(context->number())));
    return static_cast<Node*>(program_.create_node<Value_calc>(num));
  }
  if (context->name() != nullptr) {
    auto* id =
        dynamic_cast<Name*>(std::any_cast<Node*>(visit(context->name())));
    return static_cast<Node*>(program_.create_node<Value_calc>(id));
  }
  return 0;
}

std::any Builder::visitCompare(JsParser::CompareContext* context) {
  std::string logop = context->compareoperation()->getText();
  auto* val1 = dynamic_cast<Value_types*>(
      std::any_cast<Node*>(visit(context->value_types(0))));
  auto* val2 = dynamic_cast<Value_types*>(
      std::any_cast<Node*>(visit(context->value_types(1))));
  return static_cast<Node*>(program_.create_node<Compare>(val1, logop, val2));
}

std::any Builder::visitAlert(JsParser::AlertContext* context) {
  auto* val = dynamic_cast<Value_types*>(
      std::any_cast<Node*>(visit(context->value_types())));
  return static_cast<Node*>(program_.create_node<Alert>(val));
}

}  // namespace js::ast::detail
