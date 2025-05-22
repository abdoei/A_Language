#pragma once
#include <optional>
#include <string_view>
#include <unordered_map>
#include <filesystem>
#include <sstream>
#include <fstream>
#include <cassert>
#include "utils.hpp"

namespace  AL
{
constexpr char OneCharToken[]{
  '\0',
  '(',
  ')',
  '{',
  '}',
  ':',
  ';',
  ',',
  '+',
  '-',
  '*',
  '<',
  '>',
  '!'
};

enum class TokenType : char {
  Unknown = -128,
  _EOF,

  // one and more of it mean different tokens
  Slash,
  SlashSlash,
  Equal,
  EqualEqual,
  Ampersand,
  AmpersandAmpersand,
  Pipe,
  PipePipe,

  // numbers and identifiers
  Identifier,
  Int_Number,
  Float_Number,
  String,

  // reserved keywords
  Keyword_Function,
  Keyword_Number,
  Keyword_Void,
  Keyword_If,
  Keyword_Else,
  Keyword_While,
  Keyword_Return,
  Keyword_const,
  Keyword_String,
  Keyword_Var,
  Keyword_Int,
  Keyword_Float,

  // one character tokens
  EOL = OneCharToken[0],
  LeftParenthesis = OneCharToken[1],
  RightParenthesis = OneCharToken[2],
  LeftBrace = OneCharToken[3],
  RightBrace = OneCharToken[4],
  Colon = OneCharToken[5],
  Semicolon = OneCharToken[6],
  Comma = OneCharToken[7],
  Plus = OneCharToken[8],
  Minus = OneCharToken[9],
  Asterisk = OneCharToken[10],
  LowerThan = OneCharToken[11],
  GreaterThan = OneCharToken[12],
  Exclamation = OneCharToken[13]
};

const std::unordered_map<std::string_view, TokenType> Keywords = {
    {"func", TokenType::Keyword_Function}, {"num", TokenType::Keyword_Number},
    {"void", TokenType::Keyword_Void},     {"if", TokenType::Keyword_If},
    {"else", TokenType::Keyword_Else},     {"while", TokenType::Keyword_While},
    {"return", TokenType::Keyword_Return}, {"const", TokenType::Keyword_const},
    {"string", TokenType::Keyword_String}, {"var", TokenType::Keyword_Var},
    {"int", TokenType::Keyword_Int},       {"float", TokenType::Keyword_Float}
};

/* It will not check the validity of the path if the buffer != "" */
struct SourceFile {
  std::string_view path;
  std::string buffer;

  void fill_the_buffer(){
    std::filesystem::path fs_path {path}; // Changed variable name for clarity
    if (!std::filesystem::exists(fs_path)) { // Use fs_path
      throw std::runtime_error(std::string(path) + std::string(" file not existing.\n"));
    }
    std::ifstream source_code{fs_path}; // Use fs_path
    if (!source_code) {
      throw std::runtime_error(std::string(path) + std::string(" failed to open.\n"));
    }
    std::ostringstream _buffer;
    _buffer << source_code.rdbuf(); 
    buffer = _buffer.str();
  }

  SourceFile(std::string_view p, std::string b) :
    path(p)
    {
      if(b == ""){
        fill_the_buffer();
      } else {
        buffer = std::move(b);
      }
    }
};

struct Token {
  std::optional<std::string> value = std::nullopt; // 40B
  SourceLocation loc;                              // 24B
  TokenType type;                                  // 1B
};

class Lexer {
private:
  std::unique_ptr<SourceFile> source_file;

  // counters
  unsigned int line = 1;
  unsigned int column = 0;
  unsigned int idx = 0;
  std::vector<size_t> line_start_idx; // guaranteed to have the last element = buf.size()

  // funcions
  char GetNextChar() const { return (idx >= source_file->buffer.size()) ? (char)TokenType::_EOF : source_file->buffer[idx]; };
  char PopNextChar() {
    std::string &buf = source_file->buffer;
    if (idx >= buf.size()) {
      return (char)TokenType::_EOF;
    }
    ++column;
    if(buf[idx] == '\n') {
      ++line;
      column = 0;
    }
    return buf[idx++];
  }
  SourceLocation GetSourceLocation() const {
    return {source_file->path, line, column};
  }

public:
  Lexer(const SourceFile &source)
      : source_file(std::make_unique<SourceFile>(source)) 
  {
    line_start_idx.push_back(0); // Always start with 0 for the first line.
    if (!source_file->buffer.empty()) {
      for(size_t i = 0; i < source_file->buffer.size(); ++i) { // Change loop variable type to size_t
        if (source_file->buffer[i] == '\n') {
          line_start_idx.push_back(i + 1); // Next line starts after the newline
        }
      }
      // Add the end of the buffer as the end of the last line,
      // but only if the buffer is not empty and the last character wasn't a newline (already handled).
      if (line_start_idx.back() != source_file->buffer.size() && source_file->buffer.back() != '\n') {
         line_start_idx.push_back(source_file->buffer.size());
      }
      // If the file ends with a newline, the last push_back(i+1) would already be buffer.size()
      // if the buffer is not empty and the last char is \n, then line_start_idx.back() should be buffer.size().
      // if line_start_idx.back() is i+1, and i is buffer.size() -1, then line_start_idx.back() is buffer.size().
    }
  }
  Token GetNextToken();
  std::string_view GetLine(size_t line_number) const;
};

// Helper function to convert TokenType to string
inline std::string_view to_string(TokenType type) {
  switch (type) {
    case TokenType::Unknown: return "Unknown";
    case TokenType::_EOF: return "_EOF";
    case TokenType::Slash: return "Slash";
    case TokenType::SlashSlash: return "SlashSlash";
    case TokenType::Equal: return "Equal";
    case TokenType::EqualEqual: return "EqualEqual";
    case TokenType::Ampersand: return "Ampersand";
    case TokenType::AmpersandAmpersand: return "AmpersandAmpersand";
    case TokenType::Pipe: return "Pipe";
    case TokenType::PipePipe: return "PipePipe";
    case TokenType::Identifier: return "Identifier";
    case TokenType::Int_Number: return "Int_Number";
    case TokenType::Float_Number: return "Float_Number";
    case TokenType::String: return "String";
    case TokenType::Keyword_Function: return "Keyword_Function";
    case TokenType::Keyword_Number: return "Keyword_Number";
    case TokenType::Keyword_Void: return "Keyword_Void";
    case TokenType::Keyword_If: return "Keyword_If";
    case TokenType::Keyword_Else: return "Keyword_Else";
    case TokenType::Keyword_While: return "Keyword_While";
    case TokenType::Keyword_Return: return "Keyword_Return";
    case TokenType::Keyword_const: return "Keyword_const";
    case TokenType::Keyword_String: return "Keyword_String";
    case TokenType::Keyword_Var: return "Keyword_Var";
    case TokenType::Keyword_Int: return "Keyword_Int";
    case TokenType::Keyword_Float: return "Keyword_Float";
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
    default: return "InvalidTokenType";
  }
}

} // namespace  AL
