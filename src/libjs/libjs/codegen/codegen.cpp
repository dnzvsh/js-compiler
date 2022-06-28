#include <libjs/codegen/codegen.hpp>

#include <iostream>

namespace js::codegen {

using namespace ast;  // NOLINT

auto codegenVisitor::find(const std::string& id) {
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

void codegenVisitor::visit(Statement& value) {
  result() << "target triple = \"x86_64-pc-linux-gnu\"\n\n";
  result() << "declare dso_local i32 @printf(i8*, ...)\n";
  result() << "declare dso_local i32 @__isoc99_scanf(i8*, ...)\n";
  result() << "\n @.str = private unnamed_addr constant [4 x i8] "
              "c\"%d\\0A\\00\"\n\n";
  result()
      << "@.str.1 = private unnamed_addr constant [4 x i8] c\"%d\\0A\\00\"\n";
  result() << "define dso_local i32 @main(){\n";

  auto* rl = value.value();
  rl->accept(*this);
  result() << "  ret i32 0\n}\n";
}

void codegenVisitor::visit(Elements& value) {
  auto exprs = value.value();
  for (auto* expr : exprs) {
    expr->accept(*this);
  }
}

void codegenVisitor::visit(Element& value) {
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

void codegenVisitor::visit(Value_types& value) {
  Node* r1 = value.val();

  if (r1 == nullptr) {
    std::string tmp1 = getreg();
    std::string tmp = getreg();
    result() << "  " << tmp1 << " = alloca i32\n";
    result()
        << "  " << tmp
        << " = call i32(i8 *, ...) @__isoc99_scanf(i8 * getelementptr([4 x "
           "i8], [4 x i8] * @.str.1, i64 0, i64 0),i32* "
        << tmp1 << ")\n";
    std::string tmp2 = getreg();
    result() << "  " << tmp2 << " = load i32, i32* " << tmp1 << "\n";
    reg_ = tmp2;
  } else {
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
}

void codegenVisitor::visit(Value_calc& value) {
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

void codegenVisitor::visit(Declaration& value) {
  std::string tmp = getreg();
  auto fres = find(value.id());
  fres->second.reg_ = tmp;
  result() << "  " << tmp << " = alloca i32\n";

  value.val()->accept(*this);
  result() << "  store i32 " << reg_ << ", i32* " << tmp << "\n\n";
}

void codegenVisitor::visit(Assigment& value) {
  auto fres = find(value.id());
  value.val()->accept(*this);
  result() << "  store i32 " << reg_ << ", i32* " << fres->second.reg_
           << "\n\n";
}

void codegenVisitor::visit(Number& value) {
  // std::string tmp1 = getreg();
  // result() << "  " << tmp1 << " = load i32, i32* " << value.value() <<
  // "\n";
  reg_ = value.value();
}

void codegenVisitor::visit(Name& value) {
  find(value.value());
  auto fres = find(value.value());
  std::string tmp = fres->second.reg_;
  std::string tmp1 = getreg();
  result() << "  " << tmp1 << " = load i32, i32* " << tmp << "\n";
  reg_ = tmp1;
}

void codegenVisitor::visit(Calculations& value) {
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
    std::string tmp1 = reg_;

    std::string oper;
    if (value.oper() == "+") {
      oper = "add nsw";
    } else if (value.oper() == "-") {
      oper = "sub nsw";
    } else if (value.oper() == "*") {
      oper = "mul nsw";
    } else if (value.oper() == "/") {
      oper = "sdiv";
    }

    auto* y = dynamic_cast<Calculations*>(value.op2());
    y->accept(*this);
    std::string tmp2 = reg_;
    std::string tmp = getreg();
    result() << "  " << tmp << " = " << oper << " i32 " << tmp1 << ", " << tmp2
             << '\n';
    reg_ = tmp;
  }
}

void codegenVisitor::visit(If_elem& value) {
  value.log()->accept(*this);

  int br1 = jumpnumber();
  result() << "  br i1 " << reg_ << ", label %tmp" << br1 << ", label %tmp";
  const auto temp = result().str();
  result() << "tmp" << br1 << ":\n";

  scopeup();
  value.exprs()->accept(*this);
  scopedown();
  std::stringstream num;
  num << jumpnumber();
  const auto temp2 = result().str().erase(0, temp.size());
  result().seekp(0);
  result() << temp << num.str() << '\n';
  result() << temp2 << "\n";
  result() << "  br label "
           << "%tmp" << num.str() << "\n";
  result() << "tmp" << num.str() << ":\n";
}

void codegenVisitor::visit(While_elem& value) {
  int br1 = jumpnumber();
  result() << "  br label "
           << "%tmp" << br1 << "\n";
  result() << "tmp" << br1 << ":\n";
  value.log()->accept(*this);

  std::stringstream num1;
  num1 << jumpnumber();
  result() << "  br i1 " << reg_ << ", label %tmp" << num1.str()
           << ", label %tmp";
  const auto temp = result().str();

  result() << "tmp" << num1.str() << ":\n";

  scopeup();
  value.exprs()->accept(*this);
  scopedown();

  std::stringstream num2;
  num2 << jumpnumber();
  const auto temp2 = result().str().erase(0, temp.size());
  result().seekp(0);
  result() << temp << num2.str() << '\n';
  result() << temp2 << "\n";
  result() << "  br label "
           << "%tmp" << br1 << "\n";
  result() << "tmp" << num2.str() << ":\n";
}

void codegenVisitor::visit(Compare& value) {
  value.op1()->accept(*this);
  std::string tmp1 = reg_;
  std::string oper;
  if (value.oper() == "==") {
    oper = "icmp eq";
  } else if (value.oper() == "!=") {
    oper = "icmp ne";
  } else if (value.oper() == ">") {
    oper = "icmp sgt";
  } else if (value.oper() == "<") {
    oper = "icmp slt";
  } else if (value.oper() == "<=") {
    oper = "icmp sle";
  } else if (value.oper() == ">=") {
    oper = "icmp sge";
  }
  value.op2()->accept(*this);
  std::string tmp2 = reg_;
  std::string tmp = getreg();
  result() << "  " << tmp << " = " << oper << " i32 " << tmp1 << ", " << tmp2
           << '\n';
  reg_ = tmp;
}

void codegenVisitor::visit(Alert& value) {
  value.val()->accept(*this);
  std::string tmp = getreg();
  result() << "  " << tmp
           << " = call i32 (i8*, ...) @printf(i8* getelementptr inbounds "
              "([4 x i8], [4 "
              "x i8]* @.str, i64 0, i64 0), i32 "
           << reg_ << ")\n";
}

void codegenVisitor::CreateIR(ast::Program& doc) {
  auto* start_rule = dynamic_cast<Statement*>(doc.get_node());
  start_rule->accept(*this);
}

}  // namespace js::codegen
