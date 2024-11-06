#pragma once
#include <string>
#include"lexer.hpp"

// Function to convert enum values to their string names
std::string to_string(AL::TokenType tokenT) {
  using AL::TokenType;
  switch (tokenT) {
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
    case TokenType::String: return "String";

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
    case TokenType::Keyword_Float: return "Keyword_Float";
    case TokenType::Keyword_Int: return "Keyword_Int";

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
