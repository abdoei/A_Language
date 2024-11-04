#include <iostream>
#include <filesystem>
#include "lexer.hpp"
#include <sstream>
#include <fstream>

using AL::TokenType;
std::string to_string(TokenType token);

int main(int argc, char *argv[]) {
  std::filesystem::path path {argv[1]};
  if (!std::filesystem::exists(path)) {
    return 2;
  }
  std::ifstream source_code{path};
  if (!source_code) {
        std::cerr << "Failed to open file.\n";
        return 1;
  }
  std::ostringstream buffer;
  buffer << source_code.rdbuf(); 
  std::string content = buffer.str();

  /*********************************************************/
  // std::cout << content << std::endl;
  AL::SourceFile src{path.c_str(), content};
  AL::Lexer L1(src);

  AL::Token current_token;
  while (AL::TokenType::_EOF != (current_token = L1.GetNextToken()).type){
    std::cout << to_string(current_token.type) << "\t\t";
    if (current_token.value != std::nullopt) {
      std::cout << current_token.value.value() << '\t';
    }
    std::cout << std::endl;
  }
  return 0;
}


// Function to convert enum values to their string names
std::string to_string(TokenType token) {
  switch (token) {
    case TokenType::Unknown: return "Unknown";
    case TokenType::_EOF: return "_EOF";

    // Multiple-character tokens
    case TokenType::Slash: return "Slash";
    case TokenType::SlashSlash: return "SlashSlash";
    case TokenType::Equal: return "Equal";
    case TokenType::EqualEqual: return "EqualEqual";
    case TokenType::Ampersand: return "Ampersand";
    case TokenType::AmpersandAmpersand: return "AmpersandAmpersand";
    case TokenType::Pipe: return "Pipe";
    case TokenType::PipePipe: return "PipePipe";

    // Identifiers and numbers
    case TokenType::Identifier: return "Identifier";
    case TokenType::Int_Number: return "Int_Number";
    case TokenType::Float_Number: return "Float_Number";

    // Reserved keywords
    case TokenType::Keyword_Function: return "Keyword_Function";
    case TokenType::Keyword_Number: return "Keyword_Number";
    case TokenType::Keyword_Void: return "Keyword_Void";
    case TokenType::Keyword_If: return "Keyword_If";
    case TokenType::Keyword_Else: return "Keyword_Else";
    case TokenType::Keyword_While: return "Keyword_While";
    case TokenType::Keyword_Return: return "Keyword_Return";
    case TokenType::Keyword_const: return "Keyword_const";
    case TokenType::Keyword_String: return "Keyword_String";

    // Single-character tokens
    case TokenType::EOL: return "EOL";
    case TokenType::LeftParenthesis: return "LeftParenthesis";
    case TokenType::RightParenthesis: return "RightParenthesis";
    case TokenType::LeftBrace: return "LeftBrace";
    case TokenType::RightBrace: return "RightBrace";
    case TokenType::Colon: return "Colon";
    case TokenType::Semicolon: return "Semicolon";
    case TokenType::Comma: return "Comma";
    case TokenType::Plus: return "Plus";
    case TokenType::Minus: return "Minus";
    case TokenType::Asterisk: return "Asterisk";
    case TokenType::LowerThan: return "LowerThan";
    case TokenType::GreaterThan: return "GreaterThan";
    case TokenType::Exclamation: return "Exclamation";
    case TokenType::Keyword_Var: return "Keyword_Var";

    default: return "Unknown";
  }
}