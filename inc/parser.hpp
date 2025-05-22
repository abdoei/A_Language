#pragma once 
#include <memory>
#include <optional>
#include <vector> 
#include "lexer.hpp"
#include "ast.hpp"
#include <cassert>


namespace AL{
// class Lexer;
// class Token;
// class FunctionDeclaration;
// class Block;
// class Type;

class Parser{
  Lexer *lexer;
  Token next_token;
  bool incomplete_AST{false};
  
  void PopNextToken() { next_token = lexer->GetNextToken(); }
  
  // Error reporting and recovery
  void ReportError(const Token& error_token, const std::string& expectation_message);
  void SyncOn(TokenType type) { 
    // incomplete_AST is set by ReportError, which should be called before SyncOn
    while(next_token.type != type && next_token.type != TokenType::_EOF)
      PopNextToken();
  }

  // AST parsers
  std::optional<Type> ParseType();
  std::unique_ptr<Block> ParseBlock();
  std::unique_ptr<FunctionDeclaration> ParseFunctionDeclaration();

public:
  Parser(Lexer &lex)  
    : lexer(&lex), next_token(lexer->GetNextToken()) {}

  std::pair< std::vector< std::unique_ptr< FunctionDeclaration > >, bool>
  ParseSourceFile();
};  
} // namespace AL
