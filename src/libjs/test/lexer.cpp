#include <gtest/gtest.h>

#include <fstream>
#include <iostream>
#include <iterator>
#include <libjs/dump_tokens.hpp>
#include <sstream>
#include <string>

namespace js::test {

TEST(LexerSuite, Parentheses) {
  std::stringstream in("[](){}");

  std::stringstream out;
  dump_tokens(in, out);

  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tSLB '['\n"
      "Loc=<1:1>\tSRB ']'\n"
      "Loc=<1:2>\tRLP '('\n"
      "Loc=<1:3>\tRRP ')'\n"
      "Loc=<1:4>\tCLB '{'\n"
      "Loc=<1:5>\tCRB '}'\n");
}

TEST(LexerSuite, Ids) {
  std::stringstream in("$qwe _qwe _ Qwe ");

  std::stringstream out;
  dump_tokens(in, out);

  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tID '$qwe'\n"
      "Loc=<1:5>\tID '_qwe'\n"
      "Loc=<1:10>\tID '_'\n"
      "Loc=<1:12>\tID 'Qwe'\n");
}

TEST(LexerSuite, Comment) {
  std::stringstream in("id // my comment\nnewid");

  std::stringstream out;
  dump_tokens(in, out);

  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tID 'id'\n"
      "Loc=<2:0>\tID 'newid'\n");
}

TEST(LexerSuite, MultyComment) {
  std::stringstream in("id /* some comment */ newid");

  std::stringstream out;
  dump_tokens(in, out);

  EXPECT_EQ(
      out.str(),
      "Loc=<1:0>\tID 'id'\n"
      "Loc=<1:22>\tID 'newid'\n");
}

}  // namespace js::test