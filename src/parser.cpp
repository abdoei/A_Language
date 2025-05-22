#include "parser.hpp"


namespace AL {
/* BNF form */
// <type>
//  ::= 'void'
//  |   <identifier>
std::optional<Type> Parser::ParseType(){
  TokenType token_type = next_token.type;
  
  if(token_type == TokenType::Keyword_Void){
    PopNextToken(); // pop "void" keyword
    return Type::BuildVoid();
  }

  if(token_type == TokenType::Identifier) {
    assert((next_token.value != std::nullopt) && "Identifier token without value");
    AL::Type ret_type = Type::Custom(next_token.value.value());
    PopNextToken();
    return ret_type;
  }

  ReportError(next_token, "Expected type specifier (e.g., 'void' or identifier).");
  return std::nullopt;
}

/* BNF form */
// <block>
//  ::= '{' '}'
std::unique_ptr<Block> Parser::ParseBlock(){
  SourceLocation body_location = next_token.loc;
  PopNextToken(); // pop '{'

  if (next_token.type != TokenType::RightBrace) {
    ReportError(next_token, "Expected '}' at the end of the block body.");
    // Attempt to sync to the next potential function or EOF to allow further parsing.
    // SyncOn(TokenType::Keyword_Function); // Or a more suitable token for recovery here
    return nullptr; // Indicate parsing failure for this block
  }
  PopNextToken(); // pop '}'

  return std::make_unique<Block>(body_location);
}

/* BNF form */
// <FunctionDeclaration>
//  ::= 'func' <identifier> '(' ')' ':' <type> <body>
std::unique_ptr<FunctionDeclaration> Parser::ParseFunctionDeclaration(){
  SourceLocation function_location = next_token.loc;
  PopNextToken(); // pop the func keyword

  // check the <identifier>
  if (next_token.type != TokenType::Identifier) {
    ReportError(next_token, "Expected function name (identifier).");
    return nullptr;
  }
  // check if the <identifier> option has value
  assert((next_token.value != std::nullopt) && "Identifier token without value");
  std::string function_identifier = next_token.value.value();
  PopNextToken(); // pop <identifier>

  // chech the '('
  if(next_token.type != TokenType::LeftParenthesis) {
    ReportError(next_token, "Expected '(' after function name.");
    return nullptr;
  }
  PopNextToken(); // pop the '('

  // chech the ')'
  if(next_token.type != TokenType::RightParenthesis) {
    ReportError(next_token, "Expected ')' after function parameters.");
    return nullptr;
  }
  PopNextToken(); // pop the ')'

  // check the ':'
  if(next_token.type != TokenType::Colon) {
    ReportError(next_token, "Expected ':' before function return type.");
    return nullptr;
  }
  PopNextToken(); // pop the ':'

  // check for type
  std::optional<Type> type_resolved = ParseType();
  if (type_resolved == std::nullopt) {
    // ParseType already called ReportError
    return nullptr;
  }

  // chech the '{'
  if(next_token.type != TokenType::LeftBrace) {
    ReportError(next_token, "Expected '{' to start function body.");
    return nullptr;
  }
  // check for body
  std::unique_ptr<Block> body_resolved = (ParseBlock());
  if (body_resolved == nullptr) {
    return nullptr;
  }

  return std::make_unique<FunctionDeclaration>(
      function_location, function_identifier, type_resolved.value(),
      std::move(body_resolved));
}

/* BNF form */
// <sourceFile>
//  ::= <functionDecl>* EOF
std::pair<std::vector<std::unique_ptr<FunctionDeclaration>>, bool>
Parser::ParseSourceFile() {
  std::vector< std::unique_ptr< FunctionDeclaration > > function_declarations;

  while(next_token.type != TokenType::_EOF){
    if(next_token.type != TokenType::Keyword_Function){
      ReportError(next_token, "Expected 'func' keyword for function declaration at top level.");
      SyncOn(TokenType::Keyword_Function); // Try to find next function
      if(next_token.type == TokenType::_EOF) break; // Stop if EOF reached after sync
      continue;
    }

    std::unique_ptr<FunctionDeclaration> func = ParseFunctionDeclaration();
    if(func == nullptr) {
      // ParseFunctionDeclaration or one of its sub-parsers already called ReportError
      SyncOn(TokenType::Keyword_Function); // Try to find next function
      if(next_token.type == TokenType::_EOF) break; // Stop if EOF reached after sync
      continue;
    }

    function_declarations.emplace_back(std::move(func));
  }

  return {std::move(function_declarations), incomplete_AST}; // Return incomplete_AST directly
}

} // namespace AL
