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

    return {std::move(value), token_start_loc, TokenType::String};
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

std::string_view Lexer::GetLine(size_t line_number) const {
  assert(line_number > 0); // Line numbers are 1-based

  // line_start_idx contains start indices of lines.
  // line_start_idx[0] is start of line 1.
  // line_start_idx[k] is start of line k+1 (0-indexed k).
  // The last element of line_start_idx is source_file->buffer.size().
  // Number of actual lines = line_start_idx.size() - 1 (if > 0), or 1 if buffer is empty (line_start_idx = [0]).

  if (line_start_idx.empty()) { // Should not happen if constructor ran
    return {};
  }
  
  if (line_number == 0 || line_number > (line_start_idx.size() - (line_start_idx.back() == source_file->buffer.size() ? 1:0) ) ) {
     // If line_start_idx.back() is buffer.size(), then .size()-1 is the number of lines.
     // Example: {"Hello\nWorld"} -> line_start_idx = [0, 6, 11], size=3. Lines = 2. Max line_number = 2.
     // line_number > 3-1 -> line_number > 2.
     // Example: {"Hello"} -> line_start_idx = [0, 5], size=2. Lines = 1. Max line_number = 1.
     // line_number > 2-1 -> line_number > 1.
     // Example: {""} -> line_start_idx = [0], size=1. Lines = 1 (empty line). Max line_number = 1.
     // line_number > 1-0 -> line_number > 1. (This needs to be line_number > (line_start_idx.size() == 1 ? 1 : line_start_idx.size() -1)
    if (source_file->buffer.empty() && line_number == 1 && line_start_idx.size() == 1 && line_start_idx[0] == 0) {
        return ""; // Special case: line 1 of an empty file is empty.
    }
     // A more robust check for valid line numbers:
     // A line number is valid if it's >= 1 and <= number of lines.
     // Number of lines can be considered line_start_idx.size() - 1, unless the buffer is empty (1 line).
     size_t num_lines = source_file->buffer.empty() ? 1 : line_start_idx.size() - 1;
     if (line_number == 0 || line_number > num_lines) {
        return {}; // Invalid line number
     }
  }


  size_t start_idx = line_start_idx[line_number - 1];
  // end_idx is the start of the next line, or buffer.size() if it's the last line.
  size_t end_idx = line_start_idx[line_number]; 

  size_t length = end_idx - start_idx;

  // Trim trailing newline characters (\n or \r\n) from the view
  if (length > 0) {
    if (source_file->buffer[start_idx + length - 1] == '\n') {
      length--;
      if (length > 0 && source_file->buffer[start_idx + length - 1] == '\r') {
        length--;
      }
    }
  }
  
  return std::string_view(source_file->buffer.data() + start_idx, length);
}

} // namespace AL
