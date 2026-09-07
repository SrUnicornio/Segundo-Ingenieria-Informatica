#include <gtest/gtest.h>

#include <fstream>

#include "../lib/code_structure.h"
#include "../lib/funcions_main.h"

// Helper to write a temporary file and return its name
static std::string WriteTempFile(const std::string& content,
                                 const std::string& name) {
  std::string path = std::string("/tmp/") + name;
  std::ofstream ofs(path);
  ofs << content;
  ofs.close();
  return path;
}

TEST(AnalyzeVariablesTest, IntInitEquals) {
  std::string src = "int a = 0;\n";
  std::string file = WriteTempFile(src, "test_int_eq.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto vars = cs.GetVariables();
  ASSERT_EQ(vars.size(), 1);
  EXPECT_EQ(vars[0].GetName(), "a");
  EXPECT_TRUE(vars[0].IsInitialized());
  EXPECT_EQ(vars[0].GetInitialValue(), "= 0");
}

TEST(AnalyzeVariablesTest, IntInitBrace) {
  std::string src = "int b{0};\n";
  std::string file = WriteTempFile(src, "test_int_brace.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto vars = cs.GetVariables();
  ASSERT_EQ(vars.size(), 1);
  EXPECT_EQ(vars[0].GetName(), "b");
  EXPECT_TRUE(vars[0].IsInitialized());
  EXPECT_EQ(vars[0].GetInitialValue(), "= 0");
}

TEST(AnalyzeVariablesTest, IntUninitialized) {
  std::string src = "int c;\n";
  std::string file = WriteTempFile(src, "test_int_uninit.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto vars = cs.GetVariables();
  ASSERT_EQ(vars.size(), 1);
  EXPECT_EQ(vars[0].GetName(), "c");
  EXPECT_FALSE(vars[0].IsInitialized());
  EXPECT_EQ(vars[0].GetInitialValue(), "");
}

TEST(AnalyzeLoopsTest, ForWhileDoWhile) {
  std::string src =
      "for (int i = 0; i < 1; ++i) {}\nwhile (true) {}\ndo { a } while(false);\n";
  std::string file = WriteTempFile(src, "test_loops.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto loops = cs.GetLoops();
  ASSERT_EQ(loops.size(), 3);
  EXPECT_EQ(loops[0].GetType(), LoopType::FOR);
  EXPECT_EQ(loops[1].GetType(), LoopType::WHILE);
  EXPECT_EQ(loops[2].GetType(), LoopType::DO_WHILE);
}

TEST(AnalyzeConditionalsTest, IfSwitch) {
  std::string src = "if (x) {}\nswitch (y) { default: break; }\n";
  std::string file = WriteTempFile(src, "test_conditionals.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto conds = cs.GetConditionals();
  ASSERT_EQ(conds.size(), 2);
}

TEST(AnalyzeMainTest, DetectMain) {
  std::string src = "int main() { return 0; }\n";
  std::string file = WriteTempFile(src, "test_main.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  EXPECT_TRUE(cs.HasMain());
}

TEST(AnalyzeCommentsTest, SingleAndMulti) {
  std::string src = "// single line comment\n/* multi\nline */\n";
  std::string file = WriteTempFile(src, "test_comments.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto comments = cs.GetComments();
  ASSERT_EQ(comments.size(), 2);
  EXPECT_EQ(comments[0].GetType(), CommentType::SINGLE_LINE);
  EXPECT_EQ(comments[1].GetType(), CommentType::MULTI_LINE);
}

// main is provided by gtest_main when linking tests

TEST(AnalyzeMixedTest, IntInsideForAndMultipleDecls) {
  std::string src =
      "for (int i = 0, j{1}; i < 2; ++i) {}\nint a, b{2}, c = 3;\n";
  std::string file = WriteTempFile(src, "test_mixed.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto vars = cs.GetVariables();
  // Expect variables: a, b, c
  ASSERT_EQ(vars.size(), 3);
  EXPECT_EQ(vars[0].GetName(), "a");
  EXPECT_FALSE(vars[0].IsInitialized());
  EXPECT_EQ(vars[1].GetName(), "b");
  EXPECT_TRUE(vars[1].IsInitialized());
  EXPECT_EQ(vars[1].GetInitialValue(), "= 2");
  EXPECT_EQ(vars[2].GetName(), "c");
  EXPECT_TRUE(vars[2].IsInitialized());
  EXPECT_EQ(vars[2].GetInitialValue(), "= 3");
  auto loops = cs.GetLoops();
  // Expect loop: for
  ASSERT_EQ(loops.size(), 1);
  EXPECT_EQ(loops[0].GetType(), LoopType::FOR);
}

TEST(AnalyzeErrorTest, InvalidDeclarations) {
  // malformed declarations should not create variables
  std::string src = "int 123abc;\nint {x};\nint ;\n";
  std::string file = WriteTempFile(src, "test_errors.cc");
  CodeStructure cs;
  AnalyzeFile(file, cs);
  auto vars = cs.GetVariables();
  EXPECT_EQ(vars.size(), 0);
}
