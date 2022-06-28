#pragma once

#include <any>
#include <memory>
#include <string>
#include <vector>

namespace js::ast {

class Node;
class Visitor;

class Program final {
 public:
  template <class T, class... Args>
  T* create_node(Args&&... args) {
    static_assert(std::is_base_of_v<Node, T>);
    nodes_.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
    return dynamic_cast<T*>(nodes_.back().get());
  }

  void set_node(ast::Node* node) { node_ = node; }
  ast::Node* get_node() const { return node_; }

 private:
  std::vector<std::unique_ptr<Node>> nodes_;
  Node* node_ = nullptr;
};

class Node {
 public:
  virtual ~Node() = default;
  virtual void accept(Visitor& visitor) = 0;
};

class Element final : public Node {
 public:
  explicit Element(Node* elem) : elem_(elem) {}
  Node* value() const { return elem_; }
  void accept(Visitor& visitor) override;

 private:
  Node* elem_;
};

class Elements final : public Node {
 public:
  explicit Elements(std::vector<Element*> elem) : elem_(std::move(elem)) {}
  std::vector<Element*> value() const { return elem_; }
  void accept(Visitor& visitor) override;

 private:
  std::vector<Element*> elem_;
};

class Statement final : public Node {
 public:
  explicit Statement(Elements* elements) : elements_(elements) {}
  Elements* value() const { return elements_; }
  void accept(Visitor& visitor) override;

 private:
  Elements* elements_;
};

class Value_types final : public Node {
 public:
  explicit Value_types(Node* value) : value_(value) {}
  Node* val() const { return value_; }
  void accept(Visitor& visitor) override;

 private:
  Node* value_;
};

class Value_calc final : public Node {
 public:
  explicit Value_calc(Node* value) : value_(value) {}
  Node* val() const { return value_; }
  void accept(Visitor& visitor) override;

 private:
  Node* value_;
};

class Declaration final : public Node {
 public:
  explicit Declaration(std::string id, Value_types* val)
      : id_(std::move(id)), val_(val) {}
  std::string id() const { return id_; }
  Value_types* val() const { return val_; }
  void accept(Visitor& visitor) override;

 private:
  std::string id_;
  Value_types* val_;
};

class Assigment final : public Node {
 public:
  explicit Assigment(std::string id, Value_types* val)
      : id_(std::move(id)), val_(val) {}
  std::string id() const { return id_; }
  Value_types* val() const { return val_; }
  void accept(Visitor& visitor) override;

 private:
  std::string id_;
  Value_types* val_;
};

class Number final : public Node {
 public:
  explicit Number(std::string text) : text_(std::move(text)) {}
  const std::string& value() const { return text_; }
  void accept(Visitor& visitor) override;

 private:
  std::string text_;
};

class Name final : public Node {
 public:
  explicit Name(std::string text) : text_(std::move(text)) {}
  const std::string& value() const { return text_; }
  void accept(Visitor& visitor) override;

 private:
  std::string text_;
};

class Alert final : public Node {
 public:
  explicit Alert(Value_types* object) : object_(object) {}
  Value_types* val() const { return object_; }
  void accept(Visitor& visitor) override;

 private:
  Value_types* object_;
};

class Compare final : public Node {
 public:
  explicit Compare(Value_types* op1, std::string oper, Value_types* op2)
      : op1_(op1), oper_(std::move(oper)), op2_(op2) {}
  Value_types* op1() const { return op1_; }
  Value_types* op2() const { return op2_; }
  std::string oper() const { return oper_; }
  void accept(Visitor& visitor) override;

 private:
  Value_types* op1_;
  std::string oper_;
  Value_types* op2_;
};

class If_elem final : public Node {
 public:
  explicit If_elem(Compare* log, Elements* exprs) : log_(log), exprs_(exprs) {}
  Compare* log() const { return log_; }
  Elements* exprs() const { return exprs_; }
  void accept(Visitor& visitor) override;

 private:
  Compare* log_;
  Elements* exprs_;
};

class While_elem final : public Node {
 public:
  explicit While_elem(Compare* log, Elements* exprs)
      : log_(log), exprs_(exprs) {}
  Compare* log() const { return log_; }
  Elements* exprs() const { return exprs_; }
  void accept(Visitor& visitor) override;

 private:
  Compare* log_;
  Elements* exprs_;
};

class Calculations : public Node {
 public:
  explicit Calculations(Node* opA, Node* opB, std::string op)
      : operandA_(opA), operandB_(opB), op_(std::move(op)) {}
  std::string oper() const { return op_; }
  Node* op1() const { return operandA_; }
  Node* op2() const { return operandB_; }
  void accept(Visitor& visitor) override;

 private:
  Node* operandA_;
  Node* operandB_;
  std::string op_;
};

}  // namespace js::ast
