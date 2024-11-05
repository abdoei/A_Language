#include <gtest/gtest.h>
#include "lexer.cpp"
#include <filesystem>
#include <sstream>
#include <fstream>
#include <string>

AL::SourceFile src1{"sample0.AL", ""};
AL::Lexer lex1(src1);

TEST(LexerTest, sample0) { 
  using AL::Token;
  Token t = lex1.GetNextToken();
  
  EXPECT_EQ(t.type, AL::TokenType::Keyword_Function);
}
