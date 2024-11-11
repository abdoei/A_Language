#include "lexer.hpp"
#include "parser.hpp"
#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>
#include <vector>

using AL::Lexer;
using AL::Parser;
using AL::SourceFile;
using AL::Token;
using AL::TokenType;
using std::string;
using std::vector;

/* Lexer Tests */
// Trivial test0
TEST(LexerTest, sample0AL) {
  SourceFile src{"sample0.AL", ""};
  Lexer lex(src);

  Token t = lex.GetNextToken();

  EXPECT_EQ(t.type, AL::TokenType::Keyword_Function);
}

// Trivial test1
TEST(LexerTest, sample1AL) {
  SourceFile src{"sample1.AL", ""};
  Lexer lex(src);

  Token t = lex.GetNextToken();

  EXPECT_EQ(t.type, AL::TokenType::SlashSlash);
}

TEST(LexerTest, EmptyMain) {
  std::string content = R"( 
    func main() : void {
      return;
    }
    )";
  SourceFile src{"path is not checked", content};
  Lexer lex(src);

  vector<TokenType> expected, output;
  expected = {TokenType::Keyword_Function,
              TokenType::Identifier,
              TokenType::LeftParenthesis,
              TokenType::RightParenthesis,
              TokenType::Colon,
              TokenType::Keyword_Void,
              TokenType::LeftBrace,
              TokenType::Keyword_Return,
              TokenType::Semicolon,
              TokenType::RightBrace};

  Token t;
  while ((t = lex.GetNextToken()).type != TokenType::_EOF) {
    output.emplace_back(std::move(t.type));
  }

  EXPECT_EQ(expected, output);
}

TEST(LexerTest, MathInMain) {
  std::string content = R"( 
    func main() : void {
      // isPrime
      if (x == 0 || x == 1) {
          return 0;
      }

      var i = 2;
      while (!(i > x / 2)) {
          if (divides(x, i)) {
              return 0;
          }
          
          i = i + 1;
      }
      return 1;
    }
  )";
  SourceFile src{"path is not checked", content};
  Lexer lex(src);

  vector<TokenType> expected, output;
  expected = {
      TokenType::Keyword_Function, TokenType::Identifier,
      TokenType::LeftParenthesis, TokenType::RightParenthesis, TokenType::Colon,
      TokenType::Keyword_Void, TokenType::LeftBrace,
      /*Math In Main*/
      TokenType::SlashSlash, TokenType::Keyword_If, TokenType::LeftParenthesis,
      TokenType::Identifier, TokenType::EqualEqual, TokenType::Int_Number,
      TokenType::PipePipe, TokenType::Identifier, TokenType::EqualEqual,
      TokenType::Int_Number, TokenType::RightParenthesis, TokenType::LeftBrace,
      TokenType::Keyword_Return, TokenType::Int_Number, TokenType::Semicolon,
      TokenType::RightBrace, TokenType::Keyword_Var, TokenType::Identifier,
      TokenType::Equal, TokenType::Int_Number, TokenType::Semicolon,
      TokenType::Keyword_While, TokenType::LeftParenthesis,
      TokenType::Exclamation, TokenType::LeftParenthesis, TokenType::Identifier,
      TokenType::GreaterThan, TokenType::Identifier, TokenType::Slash,
      TokenType::Int_Number, TokenType::RightParenthesis,
      TokenType::RightParenthesis, TokenType::LeftBrace, TokenType::Keyword_If,
      TokenType::LeftParenthesis, TokenType::Identifier,
      TokenType::LeftParenthesis, TokenType::Identifier, TokenType::Comma,
      TokenType::Identifier, TokenType::RightParenthesis,
      TokenType::RightParenthesis, TokenType::LeftBrace,
      TokenType::Keyword_Return, TokenType::Int_Number, TokenType::Semicolon,
      TokenType::RightBrace, TokenType::Identifier, TokenType::Equal,
      TokenType::Identifier, TokenType::Plus, TokenType::Int_Number,
      TokenType::Semicolon, TokenType::RightBrace, TokenType::Keyword_Return,
      TokenType::Int_Number, TokenType::Semicolon, TokenType::RightBrace};

  Token t;
  while ((t = lex.GetNextToken()).type != TokenType::_EOF) {
    output.emplace_back(std::move(t.type));
  }

  EXPECT_EQ(expected, output);
}

TEST(LexerTest, TokenTypesCoverage) {
  std::string content = R"( 
    func faulty_function(x: int) : void {
      var a: int = 1;
      const var b: float = 0.5 + 10.;
      const var str: string = "Hello, world!";
      if(((a & x) == x) && (a|0) < a ) {a = 2*a - 1;}
      else {return;}
    }
  )";
  SourceFile src{"path is not checked", content};
  Lexer lex(src);

  vector<TokenType> expected, output;
  expected = {
    TokenType::Keyword_Function, TokenType::Identifier, TokenType::LeftParenthesis,
    TokenType::Identifier, TokenType::Colon, TokenType::Keyword_Int, 
    TokenType::RightParenthesis, TokenType::Colon, TokenType::Keyword_Void, 
    TokenType::LeftBrace, TokenType::Keyword_Var, TokenType::Identifier, 
    TokenType::Colon, TokenType::Keyword_Int, TokenType::Equal,
    TokenType::Int_Number, TokenType::Semicolon, TokenType::Keyword_const, 
    TokenType::Keyword_Var, TokenType::Identifier, TokenType::Colon, 
    TokenType::Keyword_Float, TokenType::Equal, TokenType::Float_Number,
    TokenType::Plus, TokenType::Unknown, TokenType::Semicolon,
    TokenType::Keyword_const, TokenType::Keyword_Var, TokenType::Identifier, 
    TokenType::Colon, TokenType::Keyword_String, TokenType::Equal, TokenType::String,
    TokenType::Semicolon, TokenType::Keyword_If, TokenType::LeftParenthesis,
    TokenType::LeftParenthesis, TokenType::LeftParenthesis, 
    TokenType::Identifier, TokenType::Ampersand, TokenType::Identifier, 
    TokenType::RightParenthesis, TokenType::EqualEqual, TokenType::Identifier,
    TokenType::RightParenthesis, TokenType::AmpersandAmpersand, 
    TokenType::LeftParenthesis, TokenType::Identifier, TokenType::Pipe,
    TokenType::Int_Number, TokenType::RightParenthesis, TokenType::LowerThan,
    TokenType::Identifier, TokenType::RightParenthesis, TokenType::LeftBrace,
    TokenType::Identifier, TokenType::Equal, TokenType::Int_Number, 
    TokenType::Asterisk, TokenType::Identifier, TokenType::Minus,
    TokenType::Int_Number, TokenType::Semicolon, TokenType::RightBrace,
    TokenType::Keyword_Else, TokenType::LeftBrace, TokenType::Keyword_Return,
    TokenType::Semicolon, TokenType::RightBrace, TokenType::RightBrace
  };

  Token t;
  while ((t = lex.GetNextToken()).type != TokenType::_EOF) {
    output.emplace_back(std::move(t.type));
  }

  EXPECT_EQ(expected, output);
}

/* Parser Tests */
TEST(Parser, FirstTest) {
  std::string content = R"( 
    func main() : void {
      
    }
    )";
  SourceFile src{"path_not_checked_PFT", content};
  Lexer lex(src);

  Parser parser = Parser(lex);
  auto &&[functions, complete] = parser.ParseSourceFile();

  EXPECT_TRUE(complete);
}
