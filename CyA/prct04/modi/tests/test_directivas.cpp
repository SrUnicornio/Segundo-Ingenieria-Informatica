#include <gtest/gtest.h>

#include "../lib/directivas.h"

// Small helper to run Match on a single line
static bool MatchLine(const std::string& line, Directivas& d) {
  bool inside = false;
  std::string multi;
  int start = 0;
  return Directivas::Match(line, d, 1, inside, multi, start);
}

TEST(DirectivasTest, IncludeStdAndUsr) {
  Directivas d;
  EXPECT_TRUE(MatchLine("#include <string>", d));
  EXPECT_EQ(d.GetType(), DirectivasType::INCLUDES_STL);
  EXPECT_TRUE(MatchLine("#include \"my.h\"", d));
  EXPECT_EQ(d.GetType(), DirectivasType::INCLUDES_USR);
}

TEST(DirectivasTest, DefineAndPragma) {
  Directivas d;
  EXPECT_TRUE(MatchLine("#define MAX 10", d));
  EXPECT_EQ(d.GetType(), DirectivasType::DEFINE);
  EXPECT_TRUE(MatchLine("#pragma once", d));
  EXPECT_EQ(d.GetType(), DirectivasType::PRAGMA);
}
