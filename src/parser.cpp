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

  report(next_token.loc, "Expected type specifier here", lexer->GetLine(next_token.loc.line));
  return std::nullopt;
}

/* BNF form */
// <block>
//  ::= '{' '}'
std::unique_ptr<Block> Parser::ParseBlock(){
  SourceLocation body_location = next_token.loc;
  PopNextToken(); // pop '{'

  if (next_token.type != TokenType::RightBrace) {
    return report(next_token.loc, "expected a '}' at the end of the block body",
                  lexer->GetLine(next_token.loc.line));
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
  if (next_token.type != TokenType::Identifier)
    return report(next_token.loc, "Expected identifier here", lexer->GetLine(next_token.loc.line));

  // check if the <identifier> option has value
  assert((next_token.value != std::nullopt) && "Identifier token without value");
  std::string function_identifier = next_token.value.value();
  PopNextToken(); // pop <identifier>

  // chech the '('
  if(next_token.type != TokenType::LeftParenthesis)
    return report(next_token.loc, "Expected a '(' here", lexer->GetLine(next_token.loc.line));
  PopNextToken(); // pop the '('

  // chech the ')'
  if(next_token.type != TokenType::RightParenthesis)
    return report(next_token.loc, "Expected a ')' here", lexer->GetLine(next_token.loc.line));
  PopNextToken(); // pop the ')'

  // check the ':'
  if(next_token.type != TokenType::Colon)
    return report(next_token.loc, "Expected a ':' here", lexer->GetLine(next_token.loc.line));
  PopNextToken(); // pop the ':'

  // check for type
  std::optional<Type> type_resolved = ParseType();
  if (type_resolved == std::nullopt) {
    return nullptr;
  }

  // chech the '{'
  if(next_token.type != TokenType::LeftBrace)
    return report(next_token.loc, "Expected function body here", lexer->GetLine(next_token.loc.line));

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
      report(next_token.loc, "Only functions are allowed on top level", lexer->GetLine(next_token.loc.line));
      
      SyncOn(TokenType::Keyword_Function);
      continue;
    }

    std::unique_ptr<FunctionDeclaration> func = ParseFunctionDeclaration();
    if(func == nullptr) {
      SyncOn(TokenType::Keyword_Function);
      continue;
    }

    function_declarations.emplace_back(std::move(func));
  }

  return {std::move(function_declarations), !incomplete_AST};
}

} // namespace AL
