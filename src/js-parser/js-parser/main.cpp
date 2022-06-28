#include <libjs/codegen/codegen.hpp>
#include <libjs/dump_tokens.hpp>
#include <libjs/parser.hpp>
#include <libjs/symtab/symtab.hpp>

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>
#include <string>

const char* const file_path_opt = "file_path";
const char* const dump_tokens_opt = "dump-tokens";
const char* const dump_ast_opt = "dump-ast";
const char* const dump_asm_opt = "dump-asm";

int main(int argc, char** argv) {
  cxxopts::Options options("js-parser", "ANTLR4 js parser example");

  options.positional_help("<file_path>");

  try {
    // clang-format off
    options.add_options()
        (file_path_opt, "", cxxopts::value<std::string>())
        (dump_tokens_opt, "")
        (dump_ast_opt, "")
        (dump_asm_opt, "")
        ("h,help", "Print help");
    // clang-format on
  } catch (const cxxopts::OptionSpecException& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }

  options.parse_positional({file_path_opt});

  try {
    const auto result = options.parse(argc, argv);

    if (result.count("help") > 0 || result.count(file_path_opt) != 1) {
      std::cout << options.help() << "\n";
      return 0;
    }

    std::ifstream input_stream(result[file_path_opt].as<std::string>());

    if (!input_stream.good()) {
      std::cerr << "Unable to read stream\n";
      return 1;
    }

    if (result.count(dump_tokens_opt) > 0) {
      js::dump_tokens(input_stream, std::cout);
    } else {
      auto parser_result = js::parse(input_stream);
      if (!parser_result.errors_.empty()) {
        js::dump_errors(parser_result.errors_, std::cerr);
      } else if (result.count(dump_ast_opt) > 0) {
        js::dump_ast(parser_result.program_, std::cout);
      } else {
        auto stres = js::symbol_table(parser_result.program_);
        if (!stres.errors_.empty()) {
          js::dump_errors_st(stres.errors_, std::cerr);
        } else {
          js::codegen::codegenVisitor vis(stres.globalscope_);
          vis.CreateIR(parser_result.program_);
          if (result.count(dump_asm_opt) > 0) {
            std::cout << vis.result().str();
          } else {
            std::ofstream outf("out.ll");
            outf << vis.result().str();
          }
        }
      }
    }
  }

  catch (const cxxopts::OptionException& e) {
    std::cerr << e.what() << "\n";
    return 1;
  }
}
