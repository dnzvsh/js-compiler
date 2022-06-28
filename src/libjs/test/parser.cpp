#include <gtest/gtest.h>

#include <libjs/parser.hpp>

#include <algorithm>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>

namespace js::test {

TEST(ParserSuite, SimpleVarDefinition) {
  std::stringstream in("let qwe = 123;");

  auto parse_result = js::parse(in);

  EXPECT_TRUE(parse_result.errors_.empty());

  std::stringstream ast_str;
  js::dump_ast(parse_result.program_, ast_str);

  EXPECT_EQ(
      ast_str.str(),
      "<?xml version=\"1.0\"?>\n"
      "<program>\n"
      "  <variable declaration>\n"
      "    <name>qwe</name>\n"
      "    <value>\n"
      "      <number>123</number>\n"
      "    </value>\n"
      "  </variable declaration>\n"
      "</program>\n");
}

}  // namespace js::test
