#pragma once

#include <libjs/ast/Ast.hpp>
#include <libjs/parser.hpp>

#include <JsBaseVisitor.h>

namespace js::ast::detail {

class Builder : public JsBaseVisitor {
 public:
  explicit Builder(Program& program) : program_(program) {}

  antlrcpp::Any visitProgram(JsParser::ProgramContext* context) override;
  antlrcpp::Any visitStatement(JsParser::StatementContext* context) override;
  antlrcpp::Any visitElement(JsParser::ElementContext* context) override;
  antlrcpp::Any visitElements(JsParser::ElementsContext* context) override;

  antlrcpp::Any visitAlert(JsParser::AlertContext* context) override;

  antlrcpp::Any visitValue_types(
      JsParser::Value_typesContext* context) override;
  antlrcpp::Any visitValue_calc(JsParser::Value_calcContext* context) override;

  antlrcpp::Any visitName(JsParser::NameContext* context) override;
  antlrcpp::Any visitNumber(JsParser::NumberContext* context) override;

  antlrcpp::Any visitAssigment(JsParser::AssigmentContext* context) override;
  antlrcpp::Any visitDeclaration(
      JsParser::DeclarationContext* context) override;

  antlrcpp::Any visitIf_elem(JsParser::If_elemContext* context) override;
  antlrcpp::Any visitWhile_elem(JsParser::While_elemContext* context) override;

  antlrcpp::Any visitCompare(JsParser::CompareContext* context) override;
  antlrcpp::Any visitCalculations(
      JsParser::CalculationsContext* context) override;

 private:
  ast::Program& program_;
};

}  // namespace js::ast::detail