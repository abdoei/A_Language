#include "lexer.hpp"
#include <optional>

namespace AL {
inline bool IsSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\v' || c == '\r' || c == '\n');
}
inline bool IsAlpha(char c) {
  return ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z') || c == '_';
}
inline bool IsNum(char c) { return ('0' <= c && c <= '9'); }
inline bool IsAlphaNum(char c) { return IsAlpha(c) or IsNum(c); }

Token Lexer::GetNextToken() { 
  char current_char = PopNextChar();

  while (IsSpace(current_char)) {
    current_char = PopNextChar();
  }

  SourceLocation token_start_loc = GetSourceLocation();

  if(current_char == (char)TokenType::_EOF){
    return {std::nullopt, token_start_loc, TokenType::_EOF};
  }

  // Get single-character tokens
  if (current_char == OneCharToken[0]) {
    return {std::nullopt, token_start_loc, TokenType::EOL};
  }
  if (current_char == OneCharToken[1]) {
    return {std::nullopt, token_start_loc, TokenType::LeftParenthesis};
  }
  if (current_char == OneCharToken[2]) {
    return {std::nullopt, token_start_loc, TokenType::RightParenthesis};
  }
  if (current_char == OneCharToken[3]) {
    return {std::nullopt, token_start_loc, TokenType::LeftBrace};
  }
  if (current_char == OneCharToken[4]) {
    return {std::nullopt, token_start_loc, TokenType::RightBrace};
  }
  if (current_char == OneCharToken[5]) {
    return {std::nullopt, token_start_loc, TokenType::Colon};
  }
  if (current_char == OneCharToken[6]) {
    return {std::nullopt, token_start_loc, TokenType::Semicolon};
  }
  if (current_char == OneCharToken[7]) {
    return {std::nullopt, token_start_loc, TokenType::Comma};
  }
  if (current_char == OneCharToken[8]) {
    return {std::nullopt, token_start_loc, TokenType::Plus};
  }
  if (current_char == OneCharToken[9]) {
    return {std::nullopt, token_start_loc, TokenType::Minus};
  }
  if (current_char == OneCharToken[10]) {
    return {std::nullopt, token_start_loc, TokenType::Asterisk};
  }
  if (current_char == OneCharToken[11]) {
    return {std::nullopt, token_start_loc, TokenType::LowerThan};
  }
  if (current_char == OneCharToken[12]) {
    return {std::nullopt, token_start_loc, TokenType::GreaterThan};
  }
  if (current_char == OneCharToken[13]) {
    return {std::nullopt, token_start_loc, TokenType::Exclamation};
  }

  // Capture a string
  if(current_char == '\"'){
    std::string value;

    while('\"' != (current_char = PopNextChar())){
      value += current_char;
    }

    return {std::move(value), token_start_loc, TokenType::Keyword_String};
  }

  // Get a comment start
  if(current_char == '/'){
    if (GetNextChar() == '/') {
      current_char = PopNextChar();
      std::string commnet;
      while('\n' != (current_char = PopNextChar())){
        commnet += current_char;
      }
      return {std::move(commnet), token_start_loc, TokenType::SlashSlash};
    } else {
      return {std::nullopt, token_start_loc, TokenType::Slash};
    }
  }

  // Get a = and == tokens
  if(current_char == '='){
    if (GetNextChar() == '=') {
      current_char = PopNextChar();
      return {std::nullopt, token_start_loc, TokenType::EqualEqual};
    } else {
      return {std::nullopt, token_start_loc, TokenType::Equal};
    }
  }

  // Get a & and && tokens
  if(current_char == '&'){
    if (GetNextChar() == '&') {
      current_char = PopNextChar();
      return {std::nullopt, token_start_loc, TokenType::AmpersandAmpersand};
    } else {
      return {std::nullopt, token_start_loc, TokenType::Ampersand};
    }
  }

  // Get a | and || tokens
  if(current_char == '|'){
    if (GetNextChar() == '|') {
      current_char = PopNextChar();
      return {std::nullopt, token_start_loc, TokenType::PipePipe};
    } else {
      return {std::nullopt, token_start_loc, TokenType::Pipe};
    }
  }

  // Get Identifiers and Keywords
  if(IsAlpha(current_char)){
    std::string value {current_char};
    while (IsAlphaNum(GetNextChar())) {
      value += PopNextChar();
    }

    // return a keyword token
    if (Keywords.count(value)) {
      return {value, token_start_loc, Keywords.at(value)};
    }

    return {std::move(value), token_start_loc, TokenType::Identifier};
  }

  // Get the float and integers
  if(IsNum(current_char)){
    std::string num{current_char};
    while (IsNum(GetNextChar())) {
      num += PopNextChar();
    }

    if (GetNextChar() != '.') {
      return {std::move(num), token_start_loc, TokenType::Int_Number};
    }

    num += PopNextChar();

    // Unknown token like "195. "
    if (!IsNum(GetNextChar())) {
      return Token{std::move(num), token_start_loc, TokenType::Unknown};
    }

    while (IsNum(GetNextChar())) {
      num += PopNextChar();
    }

    return {std::move(num), token_start_loc, TokenType::Float_Number};
  }

  return Token{std::nullopt, token_start_loc, TokenType::Unknown};
}

} // namespace AL
