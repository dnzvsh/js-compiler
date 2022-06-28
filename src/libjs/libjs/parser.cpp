#include <libjs/parser.hpp>

#include <libjs/ast/XmlSerializer.hpp>
#include <libjs/ast/detail/Builder.hpp>

#include <JsLexer.h>
#include <JsParser.h>

#include <fmt/format.h>
#include <iostream>

namespace js {

namespace {

class StreamErrorListener : public antlr4::BaseErrorListener {
 public:
  void syntaxError(
      antlr4::Recognizer* /*recognizer*/,
      antlr4::Token* /*offendingSymbol*/,
      size_t line,
      size_t column,
      const std::string& message,
      std::exception_ptr /*e*/) override {
    errors_.emplace_back(Error{line, column, message});
  }

  const Errors& errors() const { return errors_; }

 private:
  Errors errors_;
};

}  // namespace

ParseResult parse(std::istream& in) {
  antlr4::ANTLRInputStream stream(in);
  JsLexer lexer(&stream);
  antlr4::CommonTokenStream tokens(&lexer);
  JsParser parser(&tokens);

  StreamErrorListener error_listener;
  parser.removeErrorListeners();
  parser.addErrorListener(&error_listener);

  auto* program_parse_tree = parser.program();

  const auto& errors = error_listener.errors();
  if (!errors.empty()) {
    return ParseResult::errors(errors);
  }

  ast::Program program;
  ast::detail::Builder builder(program);

  builder.visit(program_parse_tree);

  return ParseResult::program(std::move(program));
}

void dump_ast(ast::Program& program, std::ostream& out) {
  ast::XmlSerializer::exec(program, out);
}

void dump_errors(const Errors& errors, std::ostream& out) {
  for (const auto& error : errors) {
    out << fmt::format(
        "{}:{} {}\n", error.line_, error.column_, error.message_);
  }
}

}  // namespace js
